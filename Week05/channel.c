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
if (id == -1) {
    printf("An error occured with the fork") ; 
    return 2 ; 
}
if(id == 0) { 
    close(fd[0]) ; 
    char message[MAX] ;
    printf("It is publisher.Write a message: ") ; 
    fgets(message, MAX,stdin) ; 
    if(write(fd[1], &message, strlen(message)+1) == -1) {printf("An error occured while writing"); return 3;} ; 
}else {
    close(fd[1]) ; 
    char message1[MAX] ; 
   if (read(fd[0], &message1, sizeof(message1)) == -1) {printf("An error occured while reading"); return 4;};
    close(fd[0]) ; 
    printf("It is subcriber.Here is the message: %s", message1);
}

}