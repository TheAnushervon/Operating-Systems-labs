#include <stdio.h> 
#include <unistd.h> 
#include <time.h> 
#include <sys/wait.h> 
#include <stdlib.h> 
int main (int argc, char*argv[]) {
    int n ; 
    if (argc>=1) { 
        n = atoi(argv[1]); 
        printf("%d\n", n) ; 
    } else {n = 3; }
    for (int i = 0 ; i < n ; i++) { 
        pid_t pr = fork() ; 
        if (pr == 0 ) { 
            printf("We exited from child\n") ; 
            exit(EXIT_SUCCESS) ; 
        }
        else {
            printf("Now we are in parent and will sleep\n"); 
            sleep(5) ; }
    }
    return EXIT_SUCCESS ; 

}