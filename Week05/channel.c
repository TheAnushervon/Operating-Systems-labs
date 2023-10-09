#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include<string.h>
#define MAX 1025
int main(int argc, char* argv[]){
int fd[2]; 
// fd[0] - read 
// fd[1] - writing
if(pipe(fd) == -1) {
    printf("An error occured with opening pipe") ; 
    return 1 ; 
}
int id = fork() ; 
if(id == 0) { 
    close(fd[0]) ; 
    char message[MAX] ;
    printf("It is publisher.Write a message: ") ; 
    fgets(message, MAX,stdin) ; 
    write(fd[1], &message, strlen(message)+1) ; 
}else {
    close(fd[1]) ; 
    char message1[MAX] ; 
    read(fd[0], &message1, sizeof(message1));
    close(fd[0]) ; 
    printf("It is subcriber.Here is the message: %s", message1);
}

}