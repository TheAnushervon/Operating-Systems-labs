#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <string.h>

#define MAX 256

struct Thread{
	pthread_t id;
	int i;
	char message[MAX];
};
void* print_message(void* arg){
	printf("%s\n", ((Thread*)arg)->message);
	return NULL;
}
int main(){
	printf("Enter number of threads: ");
	int n;
	scanf("%d",&n);

	struct Thread threads[n];
	for(int i = 0; i < n; i ++){
		printf("Thread %d is created\n", i);
		threads[i].i = i+1;
		sprintf(threads[i].message, "Hello from thread %d", i+1);
		pthread_create(&threads[i].id, NULL, print_message, &threads[i]);
		pthread_join(threads[i].id, NULL);
		printf("Thread %d exits\n", i+1);
	}
	return 0;
}