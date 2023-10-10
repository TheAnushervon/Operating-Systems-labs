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
	for(int i = 1; i <= n; i ++){
		printf("Thread %d is created\n", i);
		threads[i-1].i = i;
		sprintf(threads[i-1].message, "Hello from thread %d", i);
		pthread_create(&threads[i-1].id, NULL, print_message, &threads[i-1]);
		pthread_join(threads[i-1].id, NULL);
		printf("Thread %d exits\n", i);
	}
	return 0;
}