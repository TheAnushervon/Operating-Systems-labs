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
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif


void unlink_all(const char *source) {
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("Error initializing inotify");
        return;
    }

    int watch_descriptor = inotify_add_watch(inotify_fd, source, IN_CREATE | IN_DELETE);
    if (watch_descriptor == -1) {
        perror("Error adding watch to inotify");
        close(inotify_fd);
        return;
    }

    int source_fd = open(source, O_RDONLY);
    if (source_fd == -1) {
        perror("Error opening source directory");
        close(inotify_fd);
        return;
    }

    DIR *dir = fdopendir(source_fd);
    if (dir == NULL) {
        perror("Error opening source directory");
        close(inotify_fd);
        return;
    }

    int last_link_inode = -1;
    char *last_link_path = NULL;

    while (1) {
        struct inotify_event event;
        ssize_t len = read(inotify_fd, &event, sizeof(event));
        if (len == -1) {
            perror("Error reading inotify event");
            break;
        }

        if (event.mask & IN_CREATE) {
            printf("File %s created\n", event.name);

            char file_path[PATH_MAX];
            snprintf(file_path, PATH_MAX, "%s/%s", source, event.name);

            struct stat file_stat;
            if (lstat(file_path, &file_stat) == -1) {
                perror("Error getting information about the file");
                continue;
            }

            if (S_ISREG(file_stat.st_mode)) {
                if (file_stat.st_ino != last_link_inode) {
                    free(last_link_path);
                    last_link_path = malloc(PATH_MAX);
                    if (last_link_path == NULL) {
                        perror("Error allocating memory");
                        break;
                    }

                    snprintf(last_link_path, PATH_MAX, "%s_lastlink", file_path);

                    if (link(file_path, last_link_path) == -1) {
                        perror("Error creating hard link");
                        continue;
                    }

                    if (last_link_inode != -1) {
                        unlink(last_link_path);
                    }

                    last_link_inode = file_stat.st_ino;
                    printf("Created hard link: %s\n", last_link_path);
                }
            }
        } else if (event.mask & IN_DELETE) {
            printf("File %s deleted\n", event.name);
        }
    }

    free(last_link_path);
    closedir(dir);
    close(inotify_fd);
}

void find_all_hlinks(const char *path) {
    struct stat source_stat;
    if (stat(path, &source_stat) == -1) {
        perror("Error getting info about path");
        return;
    }

    printf("Hard links for %s (inode %lu): \n", path, (unsigned long)source_stat.st_ino);

    DIR *dir = opendir(path);
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

        char entry_path[PATH_MAX];
        snprintf(entry_path, PATH_MAX, "%s/%s", path, entry->d_name);

        struct stat entry_stat;
        if (lstat(entry_path, &entry_stat) == -1) {
            perror("Error getting information about the entry");
            continue;
        }

        // Check if the entry is a hard link to the same inode
        if (entry_stat.st_ino == source_stat.st_ino) {
            char absolute_path[PATH_MAX];
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
    if (symlink(source, link) == -1){
        perror("Error creating symbolic link") ; 
    }else {
        printf("Created symbolic link : %s -> %s\n", link, source); 
    }
}

char *path ; 
int main(int argc , char*argv[]) {
path = malloc (strlen(argv[1])+1) ;
path = strcpy(path, argv[1]) ; 
create_sym_link(path, "mylink") ; 
find_all_hlinks(argv[1]) ; 

}