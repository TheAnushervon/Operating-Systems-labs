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


/*void ex1_main() {
    // Task 1: Create myfile1.txt, myfile11.txt, and myfile12.txt
    FILE *file1 = fopen("myfile1.txt", "w");
    if (file1 != NULL) {
        fprintf(file1, "%s", "Hello world1");
        fclose(file1);

        link("myfile1.txt", "myfile11.txt");
        link("myfile1.txt", "myfile12.txt");
        link("myfile1.txt", "watched_directory/myfile11.txt");
        link("myfile1.txt", "watched_directory/myfile12.txt");

        printf("Task 1: Files created and linked successfully.\n");

        // Task 2: Find all hard links to myfile1.txt
        printf("\nTask 2: Hard links to myfile1.txt:\n");
        find_all_hlinks("myfile1.txt");

        // Task 3: Move myfile1.txt to /tmp/myfile1.txt
        if (rename("myfile1.txt", "/tmp/myfile1.txt") == 0) {
            printf("\nTask 3: myfile1.txt moved to /tmp/myfile1.txt successfully.\n");

            // Task 4: Modify the content of myfile11.txt
            modify_file_content("myfile11.txt", "Modified content for myfile11.txt");
            printf("\nTask 4: Content of myfile11.txt modified.\n");

            // Task 5: Create symbolic link myfile13.txt
            create_sym_link("/tmp/myfile1.txt", "myfile13.txt");
            printf("\nTask 5: Symbolic link myfile13.txt created.\n");

            // Task 6: Modify the content of /tmp/myfile1.txt
            modify_file_content("/tmp/myfile1.txt", "Modified content for /tmp/myfile1.txt");
            printf("\nTask 6: Content of /tmp/myfile1.txt modified.\n");

            // Task 7: Remove duplicates of hard links to myfile11.txt
            unlink_all("myfile11.txt");

            // Task 8: Print stat info of the kept hard link
            printf("\nTask 8: Stat info of the kept hard link:\n");
            find_all_hlinks("myfile11.txt");
        } else {
            perror("Error moving myfile1.txt to /tmp/myfile1.txt");
        }
    } else {
        perror("Error creating myfile1.txt");
    }
}*/
int main(int argc , char*argv[]) {
    pathname = argv[1]; 
    /*
    FILE* main_one = fopen("myfile.txt", "w");
    fprintf(main_one, "Hello world");
    fclose(main_one);
    FILE* second_one = fopen("myfile1.txt", "r") ; 
    fclose(second_one) ; */
    /*
    
    */
   FILE* file1 = fopen("myfile1.txt", "w"); 
   //FILE* file2 = fopen("myfile2.txt", "w");
   link("myfile1.txt","myfile11.txt" ); 
   link("myfile1.txt", "myfile12.txt"); 
   find_all_hlinks("myfile1.txt"); 
   if((rename("myfile1.txt", "/tmp/myfile1.txt")==0)){
    printf("moved myfile1.txt successfully."); 
   }
   create_sym_link("myfile13.txt", "/tmp/myfile1.txt"); 
   // ex1_main() ; 
    //getchar();
}