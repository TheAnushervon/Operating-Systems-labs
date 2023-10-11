#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#define SIZE 30
#define SIZE_BUFFER 1024

int main(int argc, char** argv) {
    printf("Window for subscriber %s:\n", argv[1]);
    char path[SIZE] = "/tmp/ex1/s";
    strcat(path, argv[1]);
    
    char msg[SIZE_BUFFER];
    while (1) {
        int fd = open(path, O_RDONLY);
        read(fd, msg, SIZE_BUFFER);
        printf("Subscriber %s: %s\n", argv[1], msg);
        close(fd);
    }
}