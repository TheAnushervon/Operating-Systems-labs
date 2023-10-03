#include <stdio.h> 
#include <unistd.h> 
#include <math.h> 
#include <stdlib.h>
#include <sys/wait.h> 
#include <time.h> 
#define SIZE 120
#define min(a,b) ((a) < (b)  ? (a) : (b) )
int main () { 
srand(time(0)); 
int u[SIZE],v[SIZE] ; 
for (int i = 0 ; i < 120; i++) { 
    u[i] = rand()%100 ; 
    v[i] = rand()%100 ; 
} 
int n ; 
scanf("%d", &n); 
int size = ceil(120/n) ; 
int processes[n] ;
int counter = 0 ; 
int left = 0 ;
int right = size ;
int sum = 0 ;   
for (int i = 0 ; i < n ; i ++ )  { 
pid_t root = fork() ; 
if (root == 0) { 
    ///int sum = 0  ;
    for (int j = left; j<min(right,120); j++){
        sum+=u[j]+v[j] ; 
    }
    left+=size ; right+=size ;
    //processes[i] = sum; 
    exit(EXIT_SUCCESS) ;  
}else {processes[i] = sum ; sum = 0 ;}
}
printf("Total sum will be : %d",processes[n-1] ); 
}





