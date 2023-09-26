#include <stdio.h>
#define MAX_NAME 64
#define MAX_PATH 2048
#define MAX_DIR 1024

struct Directory;
struct File;

typedef struct File
{
    short int id;
    char file_name[MAX_NAME];
    int size;
    char *data;
    struct Directory *directory; // The parent directory
} File;

typedef struct Directory
{
    int nf;
    int nd;
    File files[256];
    struct Directory *sub_dirs[MAX_DIR];
    char path[MAX_PATH];
    char name[MAX_NAME];

    //<WRITE YOUR CODE HERE>;
} Directory;

int main(){

    return 0 ; 
}