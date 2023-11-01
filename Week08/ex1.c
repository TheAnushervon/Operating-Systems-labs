#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>


const char* pass = "pass:";
int size = 14 ; 
int left = 5 ; 
int right = 13 ; 
char* generate_password() {
    char* password = (char*)malloc(size);
   

    const char arr[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    const int num_characters = strlen(arr);

    srand(time(NULL));
    strcpy(password, pass);
    int ind_let; 
    for (int i = left; i < right; i++) {
        ind_let = rand()%num_characters;
        password[i] = arr[ind_let];
    }
    password[8] = '\0';

    return password;
}

int main() {
    pid_t pid = getpid();
    
    
    FILE* pid_file = fopen("/tmp/ex1.pid", "w");
    if (pid_file != NULL) {
        fprintf(pid_file, "%d\n", pid);
        fclose(pid_file);
    }

    char* password = generate_password();

    char* shared_memory = (char*)mmap(NULL, 9, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    
    
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    strcpy(shared_memory, password);

    while (1) {
        
        sleep(1);
    }

    return 0;
}