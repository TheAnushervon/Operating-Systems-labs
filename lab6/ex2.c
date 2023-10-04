#include <stdio.h> 
#include <pthread.h> 
#include <time.h> 
#include<sys/wait.h> 
// *arg - what is send to you 
void *thread_func(void *arg) { 
    pthread_t tid = pthread_self() ; 
    printf("Thread with the id %lu", tid) ; 
    pthread_exit(NULL); 
    
    }
int main() { 
    pthread_t t1, t2; 
    pthread_create(&t1, NULL,thread_func, NULL) ; 
     pthread_create(&t1, NULL,thread_func, NULL) ; 
     sleep (1) ; 
     printf("Program complete\n"); 
    }
