#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>


const char* pass = "pass:";

char* generate_password() {
    char* password = (char*)malloc(13 + 1);
   

    const char arr[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    const int num_characters = strlen(arr);

    srand(time(NULL));
    strcpy(password, pass);
    int ind; 
    for (int i = 5; i < 13; i++) {
        ind = rand()%num_characters;
        password[i] = arr[ind];
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

    char* shared = (char*)mmap(NULL, 8 + 1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    
    
    if (shared == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    strcpy(shared, password);

    while (1) {
        
        sleep(1);
    }

    return 0;
}