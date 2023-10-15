#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h> 

    FILE *textFile ; 
    char content[200] ; 
void sig1_handler(int signal) { 
    textFile = fopen("text.txt", "r");
    
    if (textFile == NULL) {
        perror("Error opening text.txt");
        exit(1);
    }

    if (signal == SIGINT) { 
    
        while(fgets(content, sizeof(content), textFile)){
            printf("%s ",content) ; 
        }
        fflush(stdout) ; 
        fclose(textFile) ; 
    }
}
void sig2_handler (int signal) { 
    if (signal == SIGQUIT) { 
        printf("Process terminating...") ; 
        exit(0) ; 
    }
}
int main() {
    signal(SIGINT, sig1_handler) ; 
    signal(SIGQUIT, sig2_handler) ; 
    int pidFile = open("/var/run/agent.pid", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (pidFile == -1) {
        perror("Error opening PID file");
        exit(1);
    }

   
    pid_t myPID = getpid();
    dprintf(pidFile, "%d", myPID);
    close(pidFile);

    
     textFile = fopen("text.txt", "r");
    
    if (textFile == NULL) {
        perror("Error opening text.txt");
        exit(1);
    } else {
        printf("we are\n") ; 
        char line[256] ; 
        while(fgets(line, sizeof(line), textFile) != NULL){
            printf("%s", line) ; 
        }
        fflush(stdout);  
        ///printf("he\n"); 
        fclose(textFile) ; }

 /*   char line[256];
    while (fgets(line, sizeof(line), textFile)) {
        printf("%s", line);
    }
*/
    ///fclose(textFile);


    while (1) {
        sleep(360); 
    }

    return 0;
}
