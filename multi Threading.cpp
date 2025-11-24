#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define ITERATIONS 100000

long long counter = 0;  

void *increment(void *arg) {
    for (int i = 0; i < ITERATIONS; i++) {
        counter++;   
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

   
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

  
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Expected Counter Value = %d\n", NUM_THREADS * ITERATIONS);
    printf("Actual Counter Value   = %lld\n", counter);

    return 0;
}

