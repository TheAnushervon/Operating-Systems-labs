#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <sys/inotify.h>
#include <fcntl.h>

char* pathname;

ino_t get_inode(const char *fn)
{
    DIR *dir;
    struct dirent *entry;
    char path[FILENAME_MAX];
    struct stat info;
    if ((dir = opendir(pathname)) == NULL)
        perror("opendir error");
    else
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')
            {
                strcpy(path, pathname);
                strcat(path, "/");
                strcat(path, entry->d_name);
                // printf("%s\n", path);
                if (stat(path, &info) != -1 && strcmp(entry->d_name, fn) == 0)
                    return info.st_ino;
            }
        }
        closedir(dir);
    }
    return 0;
}


void unlink_all(const char *source) {
    ino_t source_inode = get_inode(source);
    // printf("%lu\n", source_inode);
    DIR *dir;
    struct dirent *entry;
    char path[FILENAME_MAX];
    struct stat info;
    if ((dir = opendir(pathname)) == NULL)
        perror("opendir error");
    else
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')
            {
                strcpy(path, pathname);
                strcat(path, "/");
                strcat(path, entry->d_name);
                // printf("%s\n", path);
                if (stat(path, &info) != -1 && source_inode == info.st_ino && strcmp(entry->d_name, source) != 0)
                    unlink(entry->d_name);
            }
        }
        closedir(dir);
    }
}

void find_all_hlinks(const char *path) {
    struct stat source_stat;
    if (stat(path, &source_stat) == -1) {
        perror("Error getting info about path");
        return;
    }

    printf("Hard links for %s (inode %lu): \n", path, (unsigned long)source_stat.st_ino);

    DIR *dir = opendir(pathname);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char entry_path[FILENAME_MAX];
        snprintf(entry_path, FILENAME_MAX, "%s/%s", pathname, entry->d_name);
        // printf("%s\n", entry_path);
        struct stat entry_stat;
        if (stat(entry_path, &entry_stat) == -1) {
            perror("Error getting information about the entry");
            continue;
        }

        // Check if the entry is a hard link to the same inode
        if (entry_stat.st_ino == source_stat.st_ino) {
            char absolute_path[FILENAME_MAX];
            if (realpath(entry_path, absolute_path) == NULL) {
                perror("Error getting absolute path");
                continue;
            }

            printf("Inode: %lu, Path: %s\n", (unsigned long)entry_stat.st_ino, absolute_path);
        }
    }

    closedir(dir);
}
void create_sym_link(const char *source, const char *link){
    //if (symlink(source, link) == -1){
      //  perror("Error creating symbolic link") ; 
    //}else {
        symlink(source, link) ; 
        printf("Created symbolic link : %s -> %s\n", link, source); 
    //}
}

int main(int argc , char*argv[]) {
    pathname = argv[1]; 
    
   FILE* file1 = fopen("myfile1.txt", "w"); 
   int d = link("myfile1.txt","myfile11.txt" ); 
   if (d == 0 ){printf("1st ok") ; }
   int h = link("myfile1.txt", "myfile12.txt");
   if (h == 0 ) {printf("\n2nd ok\n");} 

   find_all_hlinks("myfile1.txt"); 
   printf ("\n\n\n"); 

   if((rename("myfile1.txt", "/tmp/myfile1.txt")==0)){
    printf("moved myfile1.txt successfully.\n"); 
   }

   create_sym_link("myfile13.txt", "/tmp/myfile1.txt"); 

   unlink_all("myfile11.txt"); 
   struct stat fileStat ; 
   if (stat("myfile11.txt", &fileStat) == 0){
    printf("File: myfile11.txt\n"); 
    printf("Size: %ld bytes\n", fileStat.st_size); 
    printf("I-node number: %lu\n", fileStat.st_ino); 
    printf("Hard links: %lu\n", fileStat.st_nlink); 
   }
    
   // path 
   // inode 
   // ex1_main() ; 
    //getchar();
}