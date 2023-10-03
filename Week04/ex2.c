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
    u[i] = rand()%100; 
    v[i] = rand()%100; 
} 
int n ; 
scanf("%d", &n); 
int size = ceil(120/n) ; 
int processes[n] ;
int counter = 0 ; 
int left = 0 ;
int right = size ;
int  result = 0  ;   
for (int i = 0 ; i < n ; i ++ )  { 
pid_t root = fork() ; 
if (root == 0) { 
    int dotproduct = 0  ;
    for (int j = left; j<min(right,120); j++){
        dotproduct+=u[j]*v[j] ; 
    }
    ///left+=size ; right+=size ;
    //processes[i] = sum; 
    exit(dotproduct) ;   
}else {
    left+=size ; right+=size ; 
    int status ; 
   // waitpid(root, &status, 0) ;
   // processes[i] = WIFEXITED(status) ; 
}
//printf("Total sum will be : %d",processes[n-1] ); 
}int totalDotProduct = 0;
int status ; 
    for (int i = 0; i < n; i++) {
        wait(&status) ; 
        processes[i] = WIFEXITED(status) ; 
        totalDotProduct+= processes[i];
    }

    printf("Total dot product will be : %d\n", totalDotProduct);
}





