#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <signal.h> 
#include <unistd.h>
char *path ; 
volatile int shouldex = 1 ; 
void signalHandler(int signum){
    printf("Recieved SIGNINT signal.\n") ; 
   shouldex = 0 ; 
   statinfo_output() ; // function for output all 
   // stats of folder 
    exit (EXIT_SUCCESS) ;
    
}
int main(int argc, char* argv[]){
   
    path = malloc(strlen(argv[1])+1) ; 
    path = strcpy(path, argv[1]) ; 
   
    printf("%s", path) ; 
    signal(SIGINT,signalHandler) ; 
    while(shouldex){printf("d");
    fflush(stdout) ; 
    sleep(1);}
    /*
    1) Output stat info of all files 
    and folders in start
    and end 
    2) Some modification should be 
    printed out
    */
}