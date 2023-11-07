#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

pid_t mmu_pid;
pid_t pager_pid;
int page_cnt;
struct PTE* table;
int* ram;
int mmu_wt = 1;

void handle_page_fault(int page) {
    if (table[page].valid) {
        printf("It is valid page\n");
        return;
    }
    printf("It is not valid page --> page fault\n");
    printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");

    table[page].referenced = mmu_pid;
    table[page].dirty = false;

    kill(pager_pid, SIGUSR1);

    while(mmu_wt == 1);
    mmu_wt = 1;
    printf("MMU Resumed by SIGCONT signal from pager\n");
}

void print_table() {
    printf("Page Table:\n");
    for (int i = 0; i < page_cnt; i++) {
        printf("Page %d: Valid=%d Frame=%d Dirty=%d Referenced=%d\n", i,
               table[i].valid, table[i].frame, table[i].dirty,
               table[i].referenced);
    }
}

void handle_signal(){
    mmu_wt = 0;
}

int main(int argc, char* argv[]) {
    page_cnt = atoi(argv[1]);
    char* reference_string = argv[2];
    pager_pid = atoi(argv[3]);
    mmu_pid = getpid();
    
    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd, page_cnt * sizeof(struct PTE));


    size_t table_size = page_cnt * sizeof(struct PTE);

    table = (struct PTE*)mmap(NULL, table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for (int i = 0; i < page_cnt; i++) {
        table[i].valid = false;
        table[i].frame = -1;
        table[i].dirty = false;
        table[i].referenced = 0;
    }
    printf("Initialize page table\n");
    print_table();

    signal(SIGCONT, handle_signal);
    
    for (int i = 0; i < strlen(reference_string); i += 3) {
        char mode = reference_string[i];
        int page = reference_string[i + 1] - '0';

        if (mode == 'R') {
            printf("Read request for page %d\n", page);
            handle_page_fault(page);
        } else if (mode == 'W') {
            printf("Write request for page %d\n", page);
            handle_page_fault(page);
            table[page].dirty = true;
            printf("It is a write request then set the dirty field\n");
        }
        
        print_table(page_cnt);
        printf("-------------------------\n");
    }
    printf(" Done all requests.\n MMU sends SIGUSR1 to the pager.\n MMU terminates.\n");
    kill(pager_pid, SIGUSR1);

    close(fd);
    munmap(table, table_size);

    return 0;
}