#include <stdio.h> 
#include <limits.h> 
#include <float.h>
int main(){
    int a; 
    unsigned short int b; 
    signed long int c;
    float d; 
    double e; 
    a = INT_MAX; 
    b = USHRT_MAX; 
    c = LONG_MAX; 
    d = FLT_MAX; 
    e = DBL_MAX ; 
    printf("max of int: %d\n", a) ;  
    printf("max of unsigned short int: %hu\n", b); 
    printf("max of signed long int: %ld\n", c) ; 
    printf("max of float: %f\n", d) ;  
    printf("max of double: %f\n", e) ;  

    
    printf("size of int: %d \n", sizeof(a)) ;  
    printf("size of unsigned short int: %d \n",sizeof(b)); 
    printf("size of signed long int: %d \n",sizeof(c)) ; 
    printf("size of float: %d \n", sizeof(d)) ;  
    printf("size of double: %d \n", sizeof(e)) ; }