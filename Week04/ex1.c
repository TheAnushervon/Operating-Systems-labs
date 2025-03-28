#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int main(void){
    pid_t root_pid = getpid();
    clock_t t;
    int status;
    printf("root pid: %d\n", root_pid);

    t = clock();
    pid_t child_pid = fork();
    if(child_pid == 0){
        printf("First child pid %d, and its parent pid: %d\n", getpid(), root_pid);
    }else{ 
        pid_t child_pid2 = fork();
        if(child_pid2 == 0){
            printf("Second child pid %d, and its parent pid: %d\n", getpid(), root_pid);
        }else{
            clock_t init_t = clock();
            waitpid(child_pid, &status, 0);
            if(WIFEXITED(status)){
                printf("Execution time of 1st child is %lf milliseconds\n", (double)(clock() - t) / CLOCKS_PER_SEC * 1000.0);
            }
            
            clock_t wasted_time = clock() - init_t;
            waitpid(child_pid2, &status, 0);
            if(WIFEXITED(status)){
                printf("Execution time of 2nd child is %lf milliseconds\n", (double)(clock() - wasted_time - t) / CLOCKS_PER_SEC * 1000.0);
            }
        }
    }

  return 0;
}