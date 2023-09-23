#include <stdio.h> 

int main() {
 const int x = 1 ; 
 const int *q = &x ;    
 int memsel[3] ; 
 int * p  = memsel  ; 
 *p = *q ; 
 *(p+1) = *q ; 
 *(p+2) = 2 * (*q) ;  
 
 printf("%p\n%p\n%p\n",p, p+1, p+2)  ;

 if (p == memsel && p+1 == memsel+1 && p+2 == memsel+2)
  {printf("contiguos");  }
}