#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>   // for intptr_t

#define N_THREADS 5
#define ITERATIONS 100000

long long counter = 0;
pthread_mutex_t counter_mutex;

void *worker(void *arg) {
    int id = (int)(intptr_t)arg;   // SAFE cast
    for (int i = 0; i < ITERATIONS; ++i) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_mutex_unlock(&counter_mutex);
    }

    printf("Thread %d finished.\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[N_THREADS];

    pthread_mutex_init(&counter_mutex, NULL);

    for (int i = 0; i < N_THREADS; ++i) {
        if (pthread_create(&threads[i], NULL, worker, (void*)(intptr_t)i) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < N_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&counter_mutex);

    printf("\nAll threads done. Final counter = %lld (expected %lld)\n",
           counter, (long long)N_THREADS * ITERATIONS);

    return 0;
}

