#include <stdio.h> 
#include <stdlib.h> 
struct example { 
    int idx; 
    int at; 
    int bt; 
}  ; 
int main(){ 
    FILE *file ; 
    file = fopen("data.txt", "r") ; 
    char buffer[1024] ; 
    fgets(buffer, sizeof(buffer), file) ; 
    printf("%s", buffer) ; 
    fgets(buffer, sizeof(buffer), file) ; 
    printf("\n%s\n", buffer) ; 
    fgets(buffer, sizeof(buffer), file) ; 
    printf("%s\n", buffer) ;
   
    while (fgets(buffer, sizeof(buffer), file)) {
        struct example ex;

        if (sscanf(buffer, "%d %d %d", &ex.idx, &ex.at, &ex.bt) != 3) {
            printf("Error parsing the line: %s\n", buffer);
            continue;
        }

        printf("%d %d %d\n", ex.idx, ex.at, ex.bt);
    }
   /* struct example ex ; 
    int first = 1, second = 0, third = 0 ; 
    for (int i = 0 ; i < sizeof(buffer) ; i++)  { 
        char number [3] ; 
        int n_index = 0 ; 
        while(first) {
            if(buffer[i]!=' '){
                number[n_index]=buffer[i]; 
                n_index++ ; 
            }
            else {first = 0 ;second = 1 ;  ex.idx = atoi(number) ; }
        }
        while(second) {
            if(buffer[i]!=' '){
                number[n_index]=buffer[i]; 
                n_index++ ; 
            }
            else {second = 0 ;third = 1 ;  ex.idx = atoi(number) ; }
         
        }
        while(third) {
            if(buffer[i]!=' '){
                number[n_index]=buffer[i]; 
                n_index++ ; 
            }
            else {third = 0 ;  ex.idx = atoi(number) ; }
         
        }
    }
    printf("%d\n%d\n%d\n" ,ex.idx, ex.at, ex.bt) ; */
}


