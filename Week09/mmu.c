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

struct PageTableEntry {
    bool is_valid;
    int frame_number;
    bool is_dirty;
    int reference_count;
    int hit_count;
    int miss_count;
    int access_count;
    unsigned char page_age;
};

pid_t mmu_pid;
pid_t pager_pid;
int num_pages;
struct PageTableEntry* page_table;
int* ram;
int sleep_mmu = 1;

void handle_page_fault(int page) {
    page_table[page].access_count += 1;
    if (page_table[page].is_valid) {
        printf("Page %d is valid.\n", page);
        page_table[page].hit_count += 1;
        return;
    }
    printf("Page %d is not valid, page fault occurred.\n", page);
    printf("Requesting pager to load it from disk (SIGUSR1 signal) and waiting...\n");

    page_table[page].reference_count = mmu_pid;
    page_table[page].is_dirty = false;
    page_table[page].miss_count += 1;

    // Simulate page fault by signaling the pager
    kill(pager_pid, SIGUSR1);

    // Sleep until receiving a SIGCONT signal
    while (sleep_mmu == 1);
    sleep_mmu = 1;
    printf("MMU Resumed by SIGCONT signal from the pager.\n");
}

void print_page_table(int num_pages) {
    printf("Page Table:\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d: Valid=%d Frame=%d Dirty=%d Referenced=%d\n", i,
               page_table[i].is_valid, page_table[i].frame_number, page_table[i].is_dirty,
               page_table[i].reference_count);
    }
}

void handle_signal() {
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

    size_t page_table_size = num_pages * sizeof(struct PageTableEntry);

    // Truncate the file to the size of the page table
    if (ftruncate(fd, page_table_size) < 0) {
        perror("Failed to truncate the page table file");
        return 1;
    }

    // Map the page table file
    page_table = (struct PageTableEntry*)mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("Failed to mmap the page table");
        return 1;
    }

    signal(SIGCONT, handle_signal);

    for (int i = 0; i < num_pages; i++) {
        page_table[i].is_valid = false;
        page_table[i].frame_number = -1;
        page_table[i].is_dirty = false;
        page_table[i].reference_count = 0;
        page_table[i].hit_count = 0;
        page_table[i].miss_count = 0;
        page_table[i].access_count = 0;
        page_table[i].page_age = 0;
    }
    printf("Initializing the page table:\n");
    print_page_table(num_pages);

    for (int i = 0; i < strlen(reference_string); i += 2) {
        char mode = reference_string[i];
        int page = 0;
        while (i + 1 < strlen(reference_string) && reference_string[i + 1] != ' ') {
            page = page * 10 + (reference_string[i + 1] - '0');
            i += 1;
        }

        if (mode == 'R') {
            printf("Read request for page %d\n", page);
            handle_page_fault(page);
        } else if (mode == 'W') {
            printf("Write request for page %d\n", page);
            handle_page_fault(page);
            page_table[page].is_dirty = true;
            printf("It is a write request, so the dirty field is set.\n");
        }

        print_page_table(num_pages);
        printf("-------------------------\n");
    }

    // Signal the pager to terminate
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
