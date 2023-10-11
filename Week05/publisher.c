#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE_BUFFER 1024
#define SIZE 30
#define PATH "/tmp/publisher"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("There is no n given.\n");
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    int fd;
    mkfifo(PATH, 0666);
    while (1) {
        printf("Write the message:\n");
        char input[SIZE_BUFFER];
        fgets(input, SIZE_BUFFER, stdin);
        int ln = strlen(input);
        if (ln && input[ln - 1] == '\n') {
            input[ln - 1] = '\0';
        }
        for (int i = 0 ; i < n ; i++) {
            char subs_path[SIZE] ; 
            sprintf(subs_path, "/tmp/ex1/s%d", i+1) ; 
            mkfifo(PATH, 0666); 
        }
        for (int i = 0; i < n; i++) {
            if (fork() == 0) {
                char input[SIZE_BUFFER];
                int fd = open(PATH, O_RDONLY);
                read(fd, input, SIZE_BUFFER);
                close(fd);
                char subs_path[SIZE];
                sprintf(subs_path, "/tmp/ex1/s%d", i + 1);
                fd = open(subs_path, O_WRONLY);
                write(fd, input, SIZE_BUFFER);
                close(fd);
                exit(EXIT_SUCCESS);
            } else {
                int fd = open(PATH, O_WRONLY);
                write(fd, input, SIZE_BUFFER);
                close(fd);
            }
            for (int i = 0; i < n; i++) {
                wait(NULL);
            }
        }
    }
}