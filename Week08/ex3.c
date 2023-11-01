#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/time.h>
char *memory ; 
void allocateAndFillMemory() {

     memory = (char*)malloc(1000 * 1024 * 1024);
    if (memory == NULL) {
        perror("Memory allocation failed");
        exit(1);
    }

    memset(memory, 0, 1000 * 1024 * 1024);

}

void printMemoryUsage() {
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        printf("Memory usage: %ld KB\n", usage.ru_maxrss);
    } else {
        perror("Failed to get memory usage");
    }
}

int main() {
    int i;
    for (i = 0; i < 10; i++) {
        allocateAndFillMemory();
        printMemoryUsage();
        free (memory) ; 
        sleep(1);
    }

    return 0;
}