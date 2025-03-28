#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h> 
    int notstop = 1 ;  
    FILE *textFile ; 
    char content[200] ; 
void sig1_handler(int signal) { 
    textFile = fopen("text.txt", "r");
    
    if (textFile == NULL) {
        perror("Error opening text.txt");
        exit(1);
    }

    if (signal == SIGUSR1) { 
        printf("\n") ; 
        while(fgets(content, sizeof(content), textFile)){
            printf("%s",content) ; 
        }
        fflush(stdout) ; 
        fclose(textFile) ; 
    }
}
void sig2_handler (int signal) { 
    if (signal == SIGUSR2) { 
        printf("Process terminating...") ; 
        exit(0) ; 
    }
}
void sig3_handler(int signal) { 
    if (signal == SIGSTOP) { 
        printf("stopped\n"); 
        notstop = 0 ; 
    }
}
void sig4_handler (int signal) { 
    if (signal == SIGCONT) { 
        printf("continued\n") ; 
        notstop = 1 ; 
    }
}

int main() {
    signal(SIGUSR1, sig1_handler) ; 
    signal(SIGUSR2, sig2_handler) ; 
    signal(SIGSTOP, sig3_handler) ; 
    signal(SIGCONT, sig4_handler) ; 
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
        char line[256] ; 
        while(fgets(line, sizeof(line), textFile) != NULL){
            printf("%s", line) ; 
        }
        fflush(stdout);  
        fclose(textFile) ; }


    while (1) {
        if (notstop) {
        sleep(3600); }
    }

    return 0;
}
