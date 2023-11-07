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

#define MAX_FRAMES 5
#define PAGE_SIZE 8

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
    int hit_count;
    int miss_count;
    int count;
    unsigned char age;
};

pid_t mmu_pid;
pid_t pager_pid;
int num_pages;
struct PTE* page_table;
int* ram;
int sleep_mmu = 1;

void handle_page_fault(int page) {
    page_table[page].count += 1;
    if (page_table[page].valid) {
        printf("It is valid page\n");
        // page_table[page].referenced = mmu_pid;
        page_table[page].hit_count += 1;
        return;
    }
    printf("It is not valid page --> page fault\n");
    printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");

    page_table[page].referenced = mmu_pid;
    page_table[page].dirty = false;
    page_table[page].miss_count += 1;

    // Simulate page fault by signaling the pager
    kill(pager_pid, SIGUSR1);

    // Sleep until receiving a SIGCONT signal
    while(sleep_mmu == 1);
    sleep_mmu = 1;
    printf("MMU Resumed by SIGCONT signal from pager\n");
}

void print_page_table() {
    printf("Page Table:\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d: Valid=%d Frame=%d Dirty=%d Referenced=%d\n", i,
               page_table[i].valid, page_table[i].frame, page_table[i].dirty,
               page_table[i].referenced);
    }
}

void handle_signal(){
    sleep_mmu = 0;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <num_pages> <reference_string> <pager_PID>\n", argv[0]);
        return 1;
    }
 
    num_pages = atoi(argv[1]);
    char* reference_string = argv[2];
    pager_pid = atoi(argv[3]);
    mmu_pid = getpid();

    // Open the mapped file /tmp/ex2/pagetable
    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("Failed to open the page table file");
        return 1;
    }

    size_t page_table_size = num_pages * sizeof(struct PTE);

    // Truncate the file to the size of the page table
    if (ftruncate(fd, page_table_size) < 0) {
        perror("Failed to truncate the page table file");
        return 1;
    }

    // Map the page table file
    page_table = (struct PTE*)mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Failed to mmap the page table");
        return 1;
    }

    // ram = (int*)malloc(MAX_FRAMES * num_pages);

    // Initialize page table and RAM
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = false;
        page_table[i].frame = -1;
        page_table[i].dirty = false;
        page_table[i].referenced = 0;
    }
    printf("Initialize page table\n");
    print_page_table();

    // for (int i = 0; i < MAX_FRAMES * PAGE_SIZE; i++) {
    //     ram[i] = 0;
    // }

    signal(SIGCONT, handle_signal);

    for (int i = 0; i < strlen(reference_string); i += 2) {
        char mode = reference_string[i];
        int page = 0;
        while(i + 1 < strlen(reference_string) && reference_string[i + 1] != ' '){
            page = page * 10 + (reference_string[i + 1] - '0');
            i += 1;
        }

        if (mode == 'R') {
            printf("Read request for page %d\n", page);
            handle_page_fault(page);
        } else if (mode == 'W') {
            printf("Write request for page %d\n", page);
            handle_page_fault(page);
            page_table[page].dirty = true;
            printf("It is a write request then set the dirty field\n");
        }
        
        print_page_table(num_pages);
        printf("-------------------------\n");
    }
    // signal pager to terminate
    kill(pager_pid, SIGUSR1);

    // Print the hit and miss counts
    int total_hit_count = 0;
    int total_miss_count = 0;
    for (int i = 0; i < num_pages; i++) {
        total_hit_count += page_table[i].hit_count;
        total_miss_count += page_table[i].miss_count;
    }
    printf("Hit counts: %d\n", total_hit_count);
    printf("Miss counts: %d\n", total_miss_count);
    printf("Hit ratio: %.2f percent.\n", 100.0 * total_hit_count / (total_hit_count + total_miss_count));

    // Close and unmap the page table
    close(fd);
    munmap(page_table, page_table_size);
    return 0;
}