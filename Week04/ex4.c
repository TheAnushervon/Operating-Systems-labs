#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARG_LENGTH 64

void execute_command(char *command) {
    pid_t pid;
    pid = fork();

    if (pid < 0) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        char *args[MAX_ARG_LENGTH];
        int i = 0;

        char *token = strtok(command, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (execvp(args[0], args) == -1) {
            perror("Execvp error");
            exit(EXIT_FAILURE);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];

    while (1) {
        printf("MyShell> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("Input error");
            exit(EXIT_FAILURE);
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        } else {
            execute_command(input);
        }
    }

    return 0;
}
