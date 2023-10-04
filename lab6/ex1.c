#include <stdio.h> 
#include <unistd.h>
#include<sys/wait.h>
#include <stdlib.h>  
#include<string.h> 
int main(int argc, char *argv[] ){
    int fd[2] ; 
    pipe (fd) ; 
    pid_t child = fork() ; 
    if (child == 0 ) { 
        int i = 0 ; 
        char ch ; 
        char msg [100] ; 
        while(read(fd[0], &ch, sizeof(char))> 0) {
            msg[i] = ch; 
            i++; 
        }
        // fd[1] - is for writing  
        // fd[0] = is for reading 
        msg[i] = '\0'; 
        printf("Child got: %s\n", msg) ; 
        close(fd[0]) ; 
    }
    else { 
        close(fd[0]) ; 
        char *msg = argv[1] ; 
        for (int i = 0 ; i< strlen(msg) ; i++){
            write(fd[1], &msg[i],  sizeof(char)); 
        }
    }
}