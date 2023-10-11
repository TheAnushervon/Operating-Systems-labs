#include <stdio.h> 
#include <pthread.h> 
#include <time.h> 
#include<sys/wait.h> 
// *arg - what is send to you 
void *add (void *arg){
    int counter = 0 ; 
    for (int i = 0 ; i < 1000; i++) { 
        counter +=i; 
    }
}
void *sub(void *arg){
    int counter = 0 ; 
    for (int i = 0 ; i < 1000; i++) { 
        counter -=i; 
    }
}
void *thread_func(void *arg) { 
    pthread_t tid = pthread_self() ; 
    printf("Thread with the id %lu\n", tid) ; 
    pthread_exit(NULL); 
    
    }
int main() { 
    pthread_t t1, t2; 
    pthread_create(&t1, NULL,add, NULL) ; 
     pthread_create(&t1, NULL,sub, NULL) ; 
     sleep (1) ; 
     printf("Program complete\n"); 
    }
