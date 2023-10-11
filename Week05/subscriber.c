#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

#define LEN 30
#define BUF_SIZE 1024

int main(int argc, char** argv) {
    printf("Window for subscriber %s:\n", argv[1]);
    char path[LEN] = "/tmp/ex1/s";
    strcat(path, argv[1]);
    mkfifo(path, 0666);
    char msg[BUF_SIZE];
    while (1) {
        int fd = open(path, O_RDONLY);
        read(fd, msg, BUF_SIZE);
        printf("Subscriber %s: %s\n", argv[1], msg);
        close(fd);
    }
}