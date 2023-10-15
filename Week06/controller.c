#include <stdio.h> 
#include <unistd.h> 
#include <signal.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <string.h> 
char number[256] ; 
int main() { 
FILE *check = fopen("/var/run/agent.pid", "r"); 
if(fgets(number, sizeof(number), check) !=NULL) {
printf("Agent found.\n");
pid_t agent_pid = strtoul(number, NULL ,10) ; 
char command[15] ; 
while(1) { 
    printf("Choose a command {{\"read\",\"exit\", \"stop\",\"continue\"}} to send to the agent"); 
   scanf("%s", command); 
   switch (command)
   {
   case "read":
    if (kill(agent_pid, SIGUSR1)<0){
        perror("Couldn't send the signal.\n"); 
    }
    break;
   case "exit": 
   if (kill(agent_pid, SIGUSR2) < 0) { 
    perror("Couldn't send the signal.\n") ; 
   }
   break ; 
   case "stop": 
   if (kill(agent_pid, SIGSTOP) < 0) {
    perror("Couldn't send the signal.\n") ; 
   }
   break ; 
   case "continue": 
   if (kill(agent_pid, SIGCONT) < 0) { 
    perror("Coulndn't send the signal.\n") ; 
   }
   break ; 
   default:
   printf("There is no such command\n") ; 
    break;
   }
   } 
pclose(check) ; }
else {printf("Error: No agent found."); exit(0) ; }
}