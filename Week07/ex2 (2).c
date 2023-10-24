#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>

int main() {
    FILE *file = fopen("text.txt", "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    int random_fd = open("/dev/random", O_RDONLY);
    if (random_fd == -1) {
        perror("open");
        fclose(file);
        return 1;
    }

    
    long long target_size = 500LL * 1024 * 1024; 
    long sz = sysconf(_SC_PAGESIZE); 
    long chunk_size = 1024LL * sz; 

    while (target_size > 0) {
        char buffer[chunk_size];
        ssize_t bytes_read = read(random_fd, buffer, chunk_size);
        if (bytes_read == -1) {
            perror("read");
            break;
        }

        for (ssize_t i = 0; i < bytes_read && target_size > 0; i++) {
            if (isprint(buffer[i])) {
                fputc(buffer[i], file);
                target_size--;
                if (target_size % 1024 == 0) {
                    fputc('\n', file);
                }
            }
        }
    }

    fclose(file);
    close(random_fd);

    int file_fd = open("text.txt", O_RDWR);
    if (file_fd == -1) {
        perror("open");
        return 1;
    }

    off_t offset = 0;
    int total_capital_count = 0;
    while (offset < target_size) {
        void *mapped_data = mmap(NULL, chunk_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_fd, offset);
        if (mapped_data == MAP_FAILED) {
            perror("mmap");
            close(file_fd);
            return 1;
        }

        int capital_count = 0;
        for (size_t i = 0; i < chunk_size; i++) {
            if (isupper(((char *)mapped_data)[i])) {
                ((char *)mapped_data)[i] = tolower(((char *)mapped_data)[i]);
                capital_count++;
            }
        }

        total_capital_count += capital_count;

        munmap(mapped_data, chunk_size);
        offset += chunk_size;
    }

    close(file_fd);

    printf("Total Capital Count: %d\n", total_capital_count);

    return 0;
}
