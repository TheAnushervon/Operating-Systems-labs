#include <stdio.h> 


int const_tri(int *p , int n) {
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

        int temp = 0 ; 
    for (int i = 3; i <= n; i++) {
        temp = *(p) + *(p+1) + *(p+2) ; 
        *(p) = *(p+1);
        *(p+1) = *(p+2) ; 
        *(p+2) = temp ; 
    }

    return temp;
}
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
  {printf("contiguos\n");  }

int tr_n[3] = {0,1,1} ; 
int check = const_tri(tr_n,4); 
 
}