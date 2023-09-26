#include <stdio.h>
#define MAX_NAME 64
#define MAX_PATH 2048
#define MAX_DIR 1024

struct Directory;
struct File;

typedef void (*Show_file)(struct File* file);
typedef void (*Show_dir) (struct Directory *dir) ; 
typedef void (*Add_dir) (struct Direcotry *dir1, struct Directory *dir2); 
typedef void (*Add_file)(struct File *file, struct Directory* dir); 


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

    Show_dir show_dir ; 
    Show_file show_file; 
    Add_dir add_dir ; 
} Directory;

void show_file(File *file) {
    printf("%s ", file->file_name) ; 
}

void show_dir(Directory *dir)
{
    printf("\nDIRECTORY\n");
    printf("path: %s\n", dir->path);
    printf("files:\n");
    printf("[ ");
    
    for (int i = 0; i < dir->nf; i++)
    {
        show_file(&(dir->files[i]));
    }
    
    printf("]\n");
    printf("directories:\n");
    printf("{ ");
    
    for (int i = 0; i < dir->nd; i++)
    {
        show_dir(dir->sub_dirs[i]);
    }
    
    printf("}\n");
}

void add_dir(Directory *dir1, Directory *dir2)
{
    if (dir1 && dir2)
    {
        dir2->sub_dirs[dir2->nd] = dir1;
        dir2->nd++;
        char temp_path[MAX_PATH];
        
        if (strcmp(dir2->path, "/"))
        {
            strcpy(temp_path, dir2->path);
            strcat(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
        else
        {
            strcpy(temp_path, "/");
            strcat(temp_path, dir1->name);
            strcpy(dir1->path, temp_path);
        }
    }
}


int main(){

    return 0 ; 
}