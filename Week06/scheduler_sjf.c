#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <limits.h>

#define PS_MAX 10

typedef struct{
	int idx;
	int at, bt, rt, wt, ct, tat;
	int burst;
} ProcessData;

int running_process = -1;
unsigned total_time;

ProcessData data[PS_MAX];
pid_t ps[PS_MAX];

unsigned data_size;

void read_file(FILE* file) {
    char line[100];  

    data_size = 0;
    char a[64];
    fgets(a, 64, file);

    while (fgets(line, sizeof(line), file) != NULL) {
        if (data_size >= PS_MAX) {
            fprintf(stderr, "Exceeded maximum process count.\n");
            break;
        }

        if (sscanf(line, "%d %d %d", &data[data_size].idx, &data[data_size].at, &data[data_size].bt) == 3) {
            data[data_size].burst = data[data_size].bt;

            data_size++; 
        } else {
            fprintf(stderr, "Invalid data format in the input file.\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file); 
}

void suspend(pid_t process) {
    if (ps[process] > 0) {
        kill(ps[process], SIGTSTP);
    }
}

void resume(pid_t process) {
    if (ps[process] > 0) {
        kill(ps[process], SIGCONT);
    }
}

void terminate(pid_t process) {
    if (ps[process] > 0) {
        kill(ps[process], SIGTERM);
        waitpid(ps[process], NULL, 0);
        ps[process] = 0;
    }
}

void create_process(int new_process) {
    pid_t pid = fork();

   
    if (pid == 0) {
        char process_arg[10];
        snprintf(process_arg, sizeof(process_arg), "%d", new_process);
        execlp("./worker", "./worker", process_arg, (char*)NULL);
        perror("Exec error");
        exit(EXIT_FAILURE);
    } else {
        ps[new_process] = pid;
        running_process = new_process;
        printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", new_process, data[new_process].burst);
        data[new_process].rt = total_time - data[new_process].at;
    }
}

int find_next_process() {
    int location = -1;
    int min_burst_time = INT_MAX;

    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0 && data[i].at <= total_time) {
            if (data[i].burst < min_burst_time) {
                min_burst_time = data[i].burst;
                location = i;
            }
        }
    }

    if ( location == -1 || data[location].at > total_time ) {
        total_time++;
        return find_next_process();
    }

    return location;
}


void report(){
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i=0; i< data_size; i++){
        printf("process %d:\n", i);
        printf("	at=%d\n", data[i].at);
        printf("	bt=%d\n", data[i].bt);
        printf("	ct=%d\n", data[i].ct);
        printf("	wt=%d\n", data[i].wt);
        printf("	tat=%d\n", data[i].tat);
        printf("	rt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }
    
    printf("data size = %d\n", data_size);
    float avg_wt = (float)sum_wt/data_size;
    float avg_tat = (float)sum_tat/data_size;
    printf("Average results for this run:\n");
    printf("	avg_wt=%f\n", avg_wt);
    printf("	avg_tat=%f\n", avg_tat);
}

void check_burst(){
    for(int i = 0; i < data_size; i++)
        if (data[i].burst > 0)
            return;
    
    report();
    exit(EXIT_SUCCESS);
}


void schedule_handler(int signum) {
    total_time++;

    if (running_process != -1) {
        data[running_process].burst--;

        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);

        if (data[running_process].burst == 0) {
            printf("Scheduler: Terminating Process %d (Remaining Time: 0)\n", running_process);
            terminate(running_process);
            data[running_process].ct = total_time ;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            running_process = -1;
        }
    }
     if(data[running_process].burst != 0){
        return;
     }
    check_burst();

    int Nid = find_next_process();
    ProcessData next_process = data[Nid];

    if (running_process != next_process.idx) {
        if (running_process != -1) {
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            suspend(running_process);
        }

        running_process = next_process.idx;
        if (ps[running_process] == 0) {
            create_process(running_process);
        } else {
            resume(running_process);
            printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *in_file  = fopen(argv[1], "r");
    if (in_file == NULL) {   
        printf("File is not found or cannot open it!\n"); 
        exit(EXIT_FAILURE);
    } else {
        read_file(in_file);  
    }
    
    struct itimerval timer;
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
    signal(SIGALRM, schedule_handler);
    
    while(1);
}
