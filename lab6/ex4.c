#include <stdio.h> 
#include <pthread.h> 
#include <time.h> 
#include<sys/wait.h> 
#include<unistd.h> 
// *arg - what is send to you 
// Resolving the problem of the non synchrnonizing the
// changing of one variable by using two threads simul
// using the pthread_mutex_t lock and unlock 
int counter = 0 ; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
void *add (void *arg){
    int amount = 5 ; 
    for (int i = 0 ; i < 1000; i++) { 
        pthread_mutex_lock(&mutex) ; 
        counter +=amount;
        pthread_mutex_unlock(&mutex) ; 
        amount+=i ;  
    }
}
void *sub(void *arg){
    int amount = 5 ;  
        for (int i = 0 ; i < 1000; i++) { 
        pthread_mutex_lock(&mutex) ; 

        counter -=amount;

        pthread_mutex_unlock(&mutex) ; 
        amount+=i ; 
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
     printf("Here is teh counter %d\n", counter); 
    }
