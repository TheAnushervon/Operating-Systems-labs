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

struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

int ptable;
struct PTE* table;
char** ram;
char** disk;
int page_cnt;
int frame_cnt;
int access_cd = 0;
int *_frame;

void print_RAM(){
    printf("RAM array:\n");
    for (int i = 0; i < frame_cnt; i++) {
        printf("Frame %d ---> %s\n", i, ram[i]);
    }
}

void handle_signal(int signo) {
    int rd_page = -1;
    for (int i = 0; i < page_cnt; i++) {
        if (table[i].referenced != 0) {
            rd_page = i;
            break;
        }
    }
    printf("A disk access request from MMU Process (pid=%d)\n", table[rd_page].referenced);
    if (rd_page != -1) {
        pid_t mmu_pid = table[rd_page].referenced;

        printf("Page %d is referenced\n", rd_page);
        int empty_frame = -1;
        for (int i = 0; i < frame_cnt; i++) {
            if(_frame[i] == 0){
                empty_frame = i;
                break;
            }
        }

        int target_frame = -1;
        if (empty_frame != -1) {
            printf("We can allocate it to free frame %d\n", empty_frame);
            table[rd_page].frame = empty_frame;
            table[rd_page].valid = true;
            table[rd_page].dirty = false;
            table[rd_page].referenced = 0;
            _frame[empty_frame] = 1;
            strcpy(ram[empty_frame], disk[rd_page]);
            printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", rd_page, empty_frame);
        } else {
            printf("We do not have free frames in RAM\n");
            int victim_frame = rand() % frame_cnt;
            printf("Chose a random victim page %d\n", victim_frame);
            printf("Replace/Evict it with page %d to be allocated to frame %d\n", rd_page,victim_frame);
            _frame[victim_frame] = 0;
            
            printf("Copy data from the disk (page=%d) to RAM (frame=%d)\n", rd_page, victim_frame);
            int old_page = -1;
            for(int i = 0; i < frame_cnt; i ++){
                if(strcmp(ram[victim_frame], ram[i]) == 0){
                    old_page = i;
                }
            }
            strcpy(ram[victim_frame], disk[rd_page]);

            table[rd_page].frame = victim_frame;
            table[rd_page].valid = true;
            table[rd_page].dirty = false;
            table[rd_page].referenced = 0;

            if(old_page != -1){
                table[old_page].valid = false;
                table[old_page].frame = -1;
                table[old_page].referenced = 0;
                table[old_page].dirty = false;
            }
            _frame[victim_frame] = 1;
        m                                                                                               
        }
        access_cd++;
        print_RAM();

        printf("disk accesses is %d so far\n", access_cd);                                              
        printf("Resume MMU process\n");
        printf("-------------------------\n");                                                                                                                                                                                                             
        kill(mmu_pid, SIGCONT);              
    } else {
        printf("Pager is terminated. Disk accesses: %d\n", access_cd);
        close(ptable);
        munmap(table, page_cnt * sizeof(struct PTE));
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    page_cnt = atoi(argv[1]);
    frame_cnt = atoi(argv[2]);
    _frame = (int*)malloc(frame_cnt * sizeof(int));
    for(int i = 0; i < frame_cnt; i ++){
        _frame[i] = 0;
    }
    printf("pager pid: %d\n", getpid());
    ram = (char**)malloc(frame_cnt * sizeof(char*));
    printf("-------------------------\n");
    printf("Initialize RAM:\n");
for (int i = 0; i < frame_cnt; i++) {
    char* text = (char*)malloc(PAGE_SIZE * sizeof(char));
    text[0] = '\0';
    ram[i] = text;
}

    print_RAM();
    printf("-------------------------\n");
    
    disk = (char**)malloc(page_cnt * sizeof(char*));
    printf("Initialize disk:\n");
    printf("Disk array:\n");
    for (int i = 0; i < page_cnt; i++) {
        char* random_string = (char*)malloc(PAGE_SIZE * sizeof(char));
        for(int j = 0; j < PAGE_SIZE; j ++){
            random_string[j] = (char)(rand() % 26 + 'a');
        }
        random_string[PAGE_SIZE] = '\0';
        printf("Page %d ---> %s\n", i, random_string);
        
        disk[i] = random_string;
    }
    int fd = open("/tmp/ex2/pagetable", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd, page_cnt * sizeof(struct PTE));

    
    size_t page_table_size = page_cnt * sizeof(struct PTE);
    
    table = (struct PTE*)mmap(NULL, page_table_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("-------------------------\n");
    printf("Initialize page table:\n");
    for (int i = 0; i < page_cnt; i++) {
        table[i].valid = 0;
        table[i].frame = -1;
        table[i].dirty = 0;
        table[i].referenced = 0;

        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i,
               table[i].valid, table[i].frame, table[i].dirty,
               table[i].referenced);
    }
    printf("-------------------------\n");

    signal(SIGUSR1, handle_signal);

    while (1) {
        pause();
    }

    return 0;
}