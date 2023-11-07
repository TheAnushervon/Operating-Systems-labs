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

#define PAGE_SIZE 8

// Define the structure for the page table entry
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

int page_table_fd;
struct PTE* page_table;
char** ram;
char** disk;
int num_pages, num_frames;
int disk_accesses = 0;
int *used_frame;
char* pr_algoritm;

void print_page_table() {
    printf("-------------------------\n");
    printf("Updated Page Table:\n");
    for (int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i,
               page_table[i].valid, page_table[i].frame, page_table[i].dirty,
               page_table[i].referenced);
    }
    printf("-------------------------\n");
}

void print_ram_array(){
    printf("RAM array:\n");
    for (int i = 0; i < num_frames; i++) {
        printf("Frame %d ---> %s\n", i, ram[i]);
    }
}

// choose random page to replace
int random_page(struct PTE* page_table){
    int rand_index = rand() % num_frames;
    for(int i = 0; i < num_pages; i ++){
        if(page_table[i].valid && page_table[i].frame == rand_index){
            return i;
        }
    }
    return -1;
}

// choose page to replace by Not frequently used algorithm
int nfu(struct PTE* page_table){
    int min_index = -1;
    for(int i = 0; i < num_pages; i ++){
        if(!page_table[i].valid) continue;
        if(min_index == -1 || page_table[i].count < page_table[min_index].count){
            min_index = i;
        }
    }
    return min_index;
}

// choose page to replace by Aging algorithm
int aging(struct PTE* page_table){
    for(int i = 0; i < num_pages; i ++){
        if (page_table[i].valid == 1){
            page_table[i].age = page_table[i].age >> 1;
            if(page_table[i].referenced > 0){
                page_table[i].age |= (1 << 7);
                page_table[i].referenced = 0;
            }
        }
    }

    int min_index = -1;
    for(int i = 0; i < num_pages; i ++){
        if(!page_table[i].valid) continue;
        if(min_index == -1 || page_table[i].age < page_table[min_index].age){
            min_index = i;
        }
    }
    return min_index;
}

int victim_page(){
    if(strcmp(pr_algoritm, "random") == 0){
        return random_page(page_table);
    }else if(strcmp(pr_algoritm, "nfu") == 0){
        return nfu(page_table);
    }else if(strcmp(pr_algoritm, "aging") == 0){
        return aging(page_table);
    }
    return -1;
}

void handle_signal(int signo) {
    // Find a non-zero referenced page
    int page_to_load = -1;
    for (int i = 0; i < num_pages; i++) {
        if (page_table[i].referenced != 0) {
            page_to_load = i;
            break;
        }
    }
    printf("A disk access request from MMU Process (pid=%d)\n", page_table[page_to_load].referenced);
    if (page_to_load != -1) {
        pid_t mmu_pid = page_table[page_to_load].referenced;

        // Check if there are free frames
        printf("Page %d is referenced\n", page_to_load);
        int free_frame = -1;
        for (int i = 0; i < num_frames; i++) {
            if(used_frame[i] == 0){
                free_frame = i;
                break;
            }
        }

        int target_frame = -1;
        if (free_frame != -1) {
            printf("We can allocate it to free frame %d\n", free_frame);
            // Allocate a free frame
            page_table[page_to_load].frame = free_frame;
            page_table[page_to_load].valid = true;
            page_table[page_to_load].dirty = false;
            page_table[page_to_load].referenced = 0;
            used_frame[free_frame] = 1;
            strcpy(ram[free_frame], disk[page_to_load]);
        } else {
            // Choose a random victim page
            int victim_frame = page_table[victim_page()].frame;
            if(victim_frame == -1){
                printf("Error: victim frame is -1\n");
                exit(1);
            }
            used_frame[victim_frame] = 0;
            
            printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", page_to_load, victim_frame);
            int old_page = -1;
            for(int i = 0; i < num_pages; i ++){
                if(page_table[i].frame == victim_frame){
                    old_page = i;
                }
            }
            strcpy(ram[victim_frame], disk[page_to_load]);

            page_table[page_to_load].frame = victim_frame;
            page_table[page_to_load].valid = true;
            page_table[page_to_load].dirty = false;
            page_table[page_to_load].referenced = 0;

            if(old_page != -1){
                printf("Old Page %d is referenced\n", old_page);
                page_table[old_page].valid = false;
                page_table[old_page].frame = -1;
                page_table[old_page].referenced = 0;
                page_table[old_page].dirty = false;
            }
            used_frame[victim_frame] = 1;
        }
        disk_accesses++;
        print_ram_array();

        printf("%d disk accesses in total\n", disk_accesses);
        printf("Resume MMU process\n");
        printf("-------------------------\n");
        // Send a SIGCONT signal to the MMU
        kill(mmu_pid, SIGCONT);
    } else {
        // If no non-zero referenced page is found, terminate the pager
        printf("Pager is terminated. Disk accesses: %d\n", disk_accesses);
        close(page_table_fd);
        munmap(page_table, num_pages * sizeof(struct PTE));
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <num_pages> <num_frames> <algorithm_name>\n", argv[0]);
        return 1;
    }

    num_pages = atoi(argv[1]);
    num_frames = atoi(argv[2]);
    used_frame = (int*)malloc(num_frames * sizeof(int));
    pr_algoritm = (char*)malloc(10 * sizeof(char));
    pr_algoritm = argv[3];
    printf("pager pid is %d\n", getpid());
    printf("Page Replacement Algorithm: %s\n", pr_algoritm);
    for(int i = 0; i < num_frames; i ++){
        used_frame[i] = 0;
    }

    ram = (char**)malloc(num_frames * sizeof(char*));
    printf("-------------------------\n");
    printf("Initialize RAM:\n");
    for(int i = 0; i < num_frames; i ++){
        char* text = (char*)malloc(PAGE_SIZE * sizeof(char));
        text[0] = '\0';
        ram[i] = text;
    }
    print_ram_array();
    printf("-------------------------\n");
    
    disk = (char**)malloc(num_pages * sizeof(char*));
    printf("Initialize disk:\n");
    printf("Disk array:\n");
    for (int i = 0; i < num_pages; i++) {
        char* random_string = (char*)malloc(PAGE_SIZE * sizeof(char));
        for(int j = 0; j < PAGE_SIZE; j ++){
            random_string[j] = (char)(rand() % 26 + 'a');
        }
        random_string[PAGE_SIZE] = '\0';
        printf("Page %d ---> %s\n", i, random_string);
        
        disk[i] = random_string;
    }
    
    // Initialize the page table
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

    // Initialize RAM, Disk, and other resources
    printf("-------------------------\n");
    printf("Initialize page table:\n");
    for (int i = 0; i < num_pages; i++) {
        page_table[i].valid = 0;
        page_table[i].frame = -1;
        page_table[i].dirty = 0;
        page_table[i].referenced = 0;
        page_table[i].hit_count = 0;
        page_table[i].miss_count = 0;
        page_table[i].count = 0;
        page_table[i].age = 0;

        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i,
               page_table[i].valid, page_table[i].frame, page_table[i].dirty,
               page_table[i].referenced);
    }
    printf("-------------------------\n");
    

    // Set up the signal handler for SIGUSR1
    signal(SIGUSR1, handle_signal);

    // Infinite loop to wait for signals
    while (1) {
        pause();
    }

    return 0;
}