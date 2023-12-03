#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
void without_space(char *buffer, int maxLength) {
    
    // Remove spaces from the line
    int j = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] != ' ') {
            buffer[j++] = buffer[i];
        }
    }

    // Null-terminate the modified string
    buffer[j] = '\0';
    printf("%s", buffer) ; 
}
int real_size_detecting(char line[]) { 
    size_t size = strlen(line) -1  ; 
    int real_size; 
    for (int i = 0 ;i < size; i ++) { 
        if (line[i]!= ' '){real_size++;}
    }
    return real_size ; 
}
void convert_to_int(char line[], int *value){
    for (int i = 0 ;i < strlen(line); i++) { 
        value[i] = line[i] - '0'; 
    }
}
int main(){
    FILE *input = fopen("input.txt", "r"); 

    char line[256] ; 
    fgets(line, sizeof(line), input); 
    
/*    while(fgets(line , sizeof(line), input) != NULL ){
        if (line[0] == '\n'){break ; }
        printf("%s", line); 
    }*/
    size_t size = strlen(line) - 1 ; 
    int real_size  = real_size_detecting(line); 
   // read E-vector
    int *vector_E; 
    vector_E = (int*)malloc(real_size*sizeof(int)); 
    int counter = 0 ; 
    without_space(line, sizeof(line)); 
    convert_to_int(line, vector_E) ;
    
    fgets(line, sizeof(line), input) ; 

    // read A-vector 
    fgets(line, sizeof(line), input) ; 
    real_size = real_size_detecting(line) ; 
    int *vector_A ; 
    vector_A = (int*)malloc(real_size*sizeof(int)); 
    without_space(line,sizeof(line)); 
    convert_to_int(line, vector_A) ; 
    int row = 0 ; 
    // reading current allocation matrix
    while(fgets(line,sizeof(line),input)!=NULL){
        if(line[0] == '\n'){break;}
        row ++ ; 
    }
}