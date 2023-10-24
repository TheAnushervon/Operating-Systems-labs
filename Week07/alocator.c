#include<stdio.h> 
#include<string.h> 
#include <stdlib.h>
#include <time.h> 
#define MAX_SIZE 10000000

unsigned int memory[MAX_SIZE] ; 


void allocate_FirstFit(int adrs, int size) {
    for (int i = 0; i < MAX_SIZE; i++) {
        if (memory[i] == 0) {
            int blockSize = 0;
            while (i + blockSize < MAX_SIZE && memory[i + blockSize] == 0) {
                blockSize++;
                if (blockSize == size) {
                    for (int j = i; j < i + size; j++) {
                        memory[j] = adrs;
                    }
                    return;
                }
            }
        }
    }
    printf("Failed to allocate %d units of memory for adrs %d\n", size, adrs);
}

void allocate_WorstFit(unsigned int adrs, int size) {
    int largestBlockSize = 0;
    int largestBlockStart = -1;

    for (int i = 0; i < MAX_SIZE; i++) {
        int blockSize = 0;
        if (memory[i] == 0) {
            while (i < MAX_SIZE && memory[i] == 0) {
                i++;
                blockSize++;
            }
            if (blockSize > largestBlockSize) {
                largestBlockSize = blockSize;
                largestBlockStart = i - blockSize;
            }
        }
    }

    if (largestBlockStart >= 0 && largestBlockSize >= size) {
        for (int i = largestBlockStart; i < largestBlockStart + size; i++) {
            memory[i] = adrs;
        }
    }
}

void allocate_BestFit(unsigned int adrs, int size) {
    int bestBlockSize = MAX_SIZE + 1;
    int bestBlockStart = -1;

    for (int i = 0; i < MAX_SIZE; i++) {
        int blockSize = 0;
        if (memory[i] == 0) {
            while (i < MAX_SIZE && memory[i] == 0) {
                i++;
                blockSize++;
            }
            if (blockSize >= size && blockSize < bestBlockSize) {
                bestBlockSize = blockSize;
                bestBlockStart = i - blockSize;
            }
        }
    }

    if (bestBlockStart >= 0) {
        for (int i = bestBlockStart; i < bestBlockStart + size; i++) {
            memory[i] = adrs;
        }
    }
}

void clear (int adrs) { 
    for (int i = 0 ; i < MAX_SIZE; i++) { 
        if (memory[i] == adrs){
            memory[i] = 0 ; 
        }
    }
}

void check() {
    for (int i = 0 ; i < 100; i++) { 
        printf("%d\n", memory[i]) ; 
    }
}
int main() { 

memset(memory,0,sizeof(memory)); 

FILE *file = fopen("queries.txt", "r") ; 
if (file == NULL) { 
    perror("Error opening queries.txt") ; 
    return 1 ; 
}
char line[100] ; 
int adrs, size ; 
char d ; 
clock_t startTime = clock() ; 
int query = 0 ; 
while(fgets(line, sizeof(line), file)){
if (line[0] == 'e') {break;}
query++ ; 
if (line[0] == 'a') {
sscanf(line, "%s %d %d", &d,  &adrs, &size) ; 
//allocate_FirstFit(adrs, size) ; 
//allocate_WorstFit(adrs, size) ; 
allocate_BestFit(adrs,size) ; 

}
else if (line [0] == 'c'){
    sscanf(line, "%s %d",&d, &adrs) ; 
    clear(adrs) ; 
}
}
clock_t endTime = clock() ; 
fclose(file) ; 
double time = (double) (endTime - startTime) / CLOCKS_PER_SEC ; 
double execut = query/((double)(endTime-startTime)/CLOCKS_PER_SEC);  

printf("Throughput: %f", execut) ; 

}