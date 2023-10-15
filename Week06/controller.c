#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

char number[256];

int main() {
    FILE *check = fopen("/var/run/agent.pid", "r");
    
    if (check == NULL) {
        fprintf(stderr, "Error: No agent found.\n");
        exit(1);
    }
    
    if (fgets(number, sizeof(number), check) != NULL) {
        printf("Agent found.\n");
        pid_t agent_pid = strtoul(number, NULL, 10);
        char command[15];

        while (1) {
            printf("Choose a command {\"read\",\"exit\",\"stop\",\"continue\"} to send to the agent: ");
            scanf("%s", command);

            if (strcmp(command, "read") == 0) {
                if (kill(agent_pid, SIGUSR1) < 0) {
                    perror("Couldn't send the signal.\n");
                }
            } else if (strcmp(command, "exit") == 0) {
                if (kill(agent_pid, SIGUSR2) < 0) {
                    perror("Couldn't send the signal.\n");
                }
            } else if (strcmp(command, "stop") == 0) {
                if (kill(agent_pid, SIGSTOP) < 0) {
                    perror("Couldn't send the signal.\n");
                }
            } else if (strcmp(command, "continue") == 0) {
                if (kill(agent_pid, SIGCONT) < 0) {
                    perror("Couldn't send the signal.\n");
                }
            } else {
                kill (agent_pid, SIGINT) ; 
                            }
        }
        pclose(check);
    } else {
        printf("Error: No agent found.\n");
        exit(0);
    }

    return 0;
}
