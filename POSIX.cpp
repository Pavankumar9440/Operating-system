#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>   // for intptr_t

// Thread function
void* thread_function(void *arg) {
    intptr_t num = (intptr_t)arg;   // safe cast for pointer to integer

    printf("Thread %ld: started. Thread ID = %lu\n", 
           (long)num, (unsigned long)pthread_self());

    printf("Thread %ld: calling pthread_exit()\n", (long)num);
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    int ret;

    printf("=== Thread Operations Demo: create, join, equal, exit ===\n\n");

    ret = pthread_create(&t1, NULL, thread_function, (void*)1);
    printf("Main: Created thread 1\n");

    ret = pthread_create(&t2, NULL, thread_function, (void*)2);
    printf("Main: Created thread 2\n");

    if (pthread_equal(t1, t2))
        printf("Main: t1 and t2 are SAME\n");
    else
        printf("Main: t1 and t2 are DIFFERENT\n");

    pthread_join(t1, NULL);
    printf("Main: Joined with thread 1.\n");

    pthread_join(t2, NULL);
    printf("Main: Joined with thread 2.\n");

    printf("\nMain: All threads finished.\n");

    return 0;
}

