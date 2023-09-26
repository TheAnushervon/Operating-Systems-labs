#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 64
#define MY_MAX_PATH 2048
#define MAX_DIR 1024

struct Directory;
struct File;

typedef void (*Show_file)(struct File *file);
typedef void (*Show_dir)(struct Directory *dir);
typedef void (*Add_dir)(struct Directory *dir1, struct Directory *dir2);
typedef void (*Add_file)(struct File *file, struct Directory *dir);
typedef void (*Overwrite_to_file)(struct File *file, const char *str);
typedef void (*Append_to_file)(struct File *file, const char *str);
typedef void (*Printf_file)(struct File *file);

typedef struct File {
    int id;
    char *name;
    int size;
    char *data;
    struct Directory *directory;
    Overwrite_to_file overwrite_to_file;
    Append_to_file append_to_file;
    Printf_file printf_file;

} File;

typedef struct Directory {
    int nf;
    int nd;
    File files[256];
    struct Directory *sub_dirs[MAX_DIR];
    char path[MY_MAX_PATH];
    char name[MAX_NAME];

    Show_dir show_dir;
    Show_file show_file;
    Add_dir add_dir;
    Add_file add_file;
} Directory;

Directory create_directory(const char *name, Directory *parent_directory) {
    Directory directory;
    directory.nf = 0;
    directory.nd = 0;
    strcpy(directory.name, name);
    
    if (parent_directory != NULL) {
        snprintf(directory.path, MY_MAX_PATH + 1, "%s/%s", parent_directory->path, name);
    } else {
        strcpy(directory.path, "/");
    }

    return directory;
}

File add_file(int id, const char *name, Directory *directory) {
    File file;
    file.id = id;
    file.name = strdup(name);
    file.size = 0;
    file.data = NULL;
    file.directory = directory;

    directory->files[directory->nf] = file;
    directory->nf++;

    return file;
}

void overwrite_to_file(File *file, const char *str) {
    if (file->data != NULL) {
        free(file->data);
    }
    file->data = strdup(str);
    file->size = strlen(str) + 1;
}

void append_to_file(File *file, const char *str) {
    if (file->data == NULL) {
        file->data = strdup(str);
        file->size = strlen(str) + 1;
    } else {
        int old_size = file->size;
        file->size += strlen(str);
        file->data = realloc(file->data, file->size);
        strcat(file->data + old_size - 1, str);
    }
}

const char *get_name(const File *file) {
    const char *name = strrchr(file->name, '/');
    return (name == NULL) ? file->name : (name + 1);
}

void printp_file(const File *file) {
    char path[MY_MAX_PATH];
    strcpy(path, file->directory->path);

    if (strcmp(path, "/") != 0) {
        strcat(path, "/");
    }

    printf("Path of %s file = %s%s\n", get_name(file), path, get_name(file));
}


int main() {
    Directory root_directory = create_directory("/", NULL);
    Directory home = create_directory("home", &root_directory);
    Directory bin = create_directory("bin", &root_directory);
    File bash = add_file(1, "bash", &bin);
    File ex3_1 = add_file(2, "ex3_1.c", &home);
    File ex3_2 = add_file(3, "ex3_2.c", &home);

    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);");
    overwrite_to_file(&ex3_2, "//This is a comment in C language");
    overwrite_to_file(&bash, "Bourne Again Shell!!");
    append_to_file(&ex3_1, "int main(){printf(\"Hello World!\");}");

    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);

    return 0;
}
