#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define LEN 30
#define PATH "/tmp/publisher"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Did not enter n\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    int fd;
    mkfifo(PATH, 0666);
    while (1) {
        printf("Write the message:\n");
        char input[BUF_SIZE];
        fgets(input, BUF_SIZE, stdin);
        int ln = strlen(input);
        if (ln && input[ln - 1] == '\n') {
            input[ln - 1] = '\0';
        }
        for (int i = 0; i < n; i++) {
            if (fork() == 0) {
                char input[BUF_SIZE];
                int fd = open(PATH, O_RDONLY);
                read(fd, input, BUF_SIZE);
                close(fd);
                char subs_path[LEN];
                sprintf(subs_path, "/tmp/ex1/s%d", i + 1);
                fd = open(subs_path, O_WRONLY);
                write(fd, input, BUF_SIZE);
                close(fd);
                exit(EXIT_SUCCESS);
            } else {
                int fd = open(PATH, O_WRONLY);
                write(fd, input, BUF_SIZE);
                close(fd);
            }
            for (int i = 0; i < n; i++) {
                wait(NULL);
            }
        }
    }
}