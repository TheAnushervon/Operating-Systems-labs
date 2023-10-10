#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

const int BUFFER_SIZE = 1024;

int main(int argc, char* argv[]){
    int n = 3;
    if(argc > 1){
        n = atoi(argv[1]);
    }
    char* pathName = "/tmp/ex1";
    if(mkfifo(pathName, 0666) == -1){
        if(errno != EEXIST){
            printf("error: main(): can not create a fifo!\n");
            exit(1);
        }
    }
    printf("Type some text to publish1:\n");
    fflush(stdout);
    while(1){
        int fd = open(pathName, O_WRONLY);
        
        char buff[BUFFER_SIZE];
        char nbuff[BUFFER_SIZE * n];

        fgets(buff, BUFFER_SIZE, stdin);
        
        for(int i = 0; i < n; i ++){
            for(int j = 0; j < BUFFER_SIZE; j ++){
                nbuff[i * BUFFER_SIZE + j] = buff[j];
            }
        }
        if(write(fd, nbuff, sizeof(nbuff)) == -1){
            printf("error: main(): can not write to opened pipe!\n");
            exit(1);
        }

        close(fd);
        sleep(1.5);
    }

    system("rm /tmp/ex1 -f");
}

// mkfifo reference 
// https://man7.org/linux/man-pages/man3/mkfifo.3.html