#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
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
    printf("Write a message: ") ; 
    scanf("%s", message); 
    write(fd[1], &message, sizeof(MAX)); 
}else {
    close(fd[1]) ; 
    char message1[MAX] ; 
    read(fd[0], &message1, sizeof(MAX));
    close(fd[0]) ; 
    printf("Here is the message from subscriber: %s", message1);
}

}