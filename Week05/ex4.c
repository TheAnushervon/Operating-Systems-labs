#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool is_prime(int n);

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int k = 0;
int c = 0;

int n = 0;

int get_number_to_check() {
    int ret;
    pthread_mutex_lock(&global_lock);
    ret = k;
    if (k != n) {
        k++;
    }
    pthread_mutex_unlock(&global_lock);
    return ret;
}

void increment_primes() {
    pthread_mutex_lock(&global_lock);
    c++;
    pthread_mutex_unlock(&global_lock);
}

void *check_primes(void *arg) {
    while (1) {
        int prime_to_check = get_number_to_check();
        if (prime_to_check == n) {
            break;
        }
        
        int res = is_prime(prime_to_check);

        if (res == 1) {
            increment_primes();
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int n_threads = atoi(argv[2]);
    n = atoi(argv[1]);

    pthread_t *threads = malloc(n_threads * sizeof(pthread_t));
    if (threads == NULL) {
        printf("error: main(): memory allocation failed!\n");
        return 1;
    }

    for (int i = 0; i < n_threads; i++) {
        if (pthread_create(&threads[i], NULL, check_primes, NULL) != 0) {
            printf("error: main(): cannot create threads!\n");
            return 1;
        }
    }

    for (int i = 0; i < n_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("error: main(): cannot join threads!\n");
            return 1;
        }
    }

    free(threads);

    printf("%d\n", c);
    exit(EXIT_SUCCESS);
}
