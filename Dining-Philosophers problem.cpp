#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 5   // number of philosophers

pthread_mutex_t chopstick[NUM];

void* philosopher(void* num) {
    int id = *((int*)num);   // FIXED

    while (1) {
        printf("Philosopher %d is thinking...\n", id);
        sleep(1);

        // Pick left chopstick
        pthread_mutex_lock(&chopstick[id]);
        // Pick right chopstick
        pthread_mutex_lock(&chopstick[(id + 1) % NUM]);

        printf("Philosopher %d is eating...\n", id);
        sleep(1);

        // Release both chopsticks
        pthread_mutex_unlock(&chopstick[id]);
        pthread_mutex_unlock(&chopstick[(id + 1) % NUM]);

        printf("Philosopher %d finished eating.\n", id);
    }
    return NULL;
}

int main() {
    pthread_t thread[NUM];
    int ids[NUM];

    for (int i = 0; i < NUM; i++)
        pthread_mutex_init(&chopstick[i], NULL);

    for (int i = 0; i < NUM; i++) {
        ids[i] = i;
        pthread_create(&thread[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < NUM; i++)
        pthread_join(thread[i], NULL);

    return 0;
}

