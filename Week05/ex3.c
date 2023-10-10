#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}
typedef struct prime_request {
    int a, b;
} prime_request;

void *prime_counter(void *arg) {
    prime_request req = *((prime_request *)arg); 
    int count = 0;

    for (int i = req.a; i < req.b; i++) {
        if (is_prime(i)) {
            count++;
        }
    }

    int *result = (int *)malloc(sizeof(int));
    *result = count;
    return (void *)result;
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int segment_size = n / m;

    pthread_t *threads = (pthread_t *)malloc(m * sizeof(pthread_t));
    prime_request *requests = (prime_request *)malloc(m * sizeof(prime_request));
    void **results = (void **)malloc(m * sizeof(void *));

    for (int i = 0; i < m; i++) {
        requests[i].a = i * segment_size;
        requests[i].b = (i == m - 1) ? n : (i + 1) * segment_size;

        if (pthread_create(&threads[i], NULL, prime_counter, &requests[i]) != 0) {
            printf("Error: main(): Cannot create threads!\n");
            return 1;
        }
    }

    int total_result = 0;

    for (int i = 0; i < m; i++) {
        if (pthread_join(threads[i], &results[i]) != 0) {
            printf("Error: main(): Cannot join threads!\n");
            return 1;
        }
        total_result += *((int *)results[i]);
        free(results[i]); 
    }

    free(results);
    free(requests);
    free(threads);

    printf("%d\n", total_result);

    return 0;
}
