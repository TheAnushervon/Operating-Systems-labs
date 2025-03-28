#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <errno.h>
#include <sys/types.h>
char *path;
volatile int shouldex = 1;
DIR *directory;
#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

void traverse(const char *fn)
{
    DIR *dir;
    struct dirent *entry;
    char path[1024];
    struct stat info;

    printf("Directory structure of %s:\n", fn);

    if ((dir = opendir(fn)) == NULL)
        perror("opendir error");
    else
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] != '.')
            {
                strcpy(path, fn);
                strcat(path, "/");
                strcat(path, entry->d_name);

                if (stat(path, &info) != 0)
                    fprintf(stderr, "stat() error on %s\n", path);
                printf("%s ", path);
                printf("I-node number: %lu ", info.st_ino);
                printf("Hard links: %lu\n", info.st_nlink);
            }
        }
        closedir(dir);
    }
}

void signalHandler(int signum)
{
    printf("Recieved SIGNINT signal.\n");
    shouldex = 0;
    traverse(path);
    // statinfo_output() ; // function for output all
    // stats of folder
    exit(EXIT_SUCCESS);
}
int main(int argc, char *argv[])
{

    path = malloc(strlen(argv[1]) + 1);
    path = strcpy(path, argv[1]);
    directory = opendir(path);
    printf("%s", path);
    signal(SIGINT, signalHandler);
    /*while(shouldex){printf("d");
    fflush(stdout) ;
    sleep(1);}*/

    /*
    1) Output stat info of all files
    and folders in start
    and end
    2) Some modification should be
    printed out
    */
    int length;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    /* Print the stat info of all files and folders in the path on startup */
    traverse(path);

    /* Creating the INOTIFY instance */
    fd = inotify_init();

    /* Checking for error */
    if (fd < 0)
    {
        perror("inotify_init");
        return EXIT_FAILURE;
    }

    /* Adding the specified directory into the watch list. */
    wd = inotify_add_watch(fd, path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_ATTRIB | IN_OPEN | IN_ACCESS);

    /* Read to determine the event change happens on the specified directory. */
    while (1)
    {
        int i = 0;
        length = read(fd, buffer, EVENT_BUF_LEN);

        /* Checking for error */
        if (length < 0)
        {
            perror("read");
            close(fd);
            return EXIT_FAILURE;
        }

        /* Actually read returns the list of change events happens. */
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len)
            {
                /// printf("File or directory %s ", event->name);
                // IN_ISDIR
                if (event->mask & IN_ACCESS)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s was accessed.\n", event->name);
                    }
                    else
                    {
                        printf("File %s was accessed.\n", event->name);
                    }
                }
                if (event->mask & IN_CREATE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("New directory %s was created.\n", event->name);
                    }
                    else
                    {
                        printf("New file %s was created.\n", event->name);
                    }
                }
                if (event->mask & IN_DELETE)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s was deleted.\n", event->name);
                    }
                    else
                    {
                        printf("File %s was deleted.\n", event->name);
                    }
                }
                if (event->mask & IN_MODIFY)
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s was modified.\n", event->name);
                    }
                    else
                    {
                        printf("File %s was modified.\n", event->name);
                    }
                if (event->mask & IN_OPEN)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s was opened.\n", event->name);
                    }
                    else
                    {
                        printf("File %s was opened.\n", event->name);
                    }
                }
                if (event->mask & IN_ATTRIB)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("Directory %s metadata changed.\n", event->name);
                    }
                    else
                    {
                        printf("File %s metadata changed.\n", event->name);
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    /* Print the stat info of all files and folders in the path before termination */
    traverse(path);

    /* Removing the specified directory from the watch list. */
    inotify_rm_watch(fd, wd);

    /* Closing the INOTIFY instance. */
    close(fd);

    return EXIT_SUCCESS;

    printf("Here");
    traverse(path);
}