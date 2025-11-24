#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>     
#include <time.h>

#define N_READERS 4
#define N_WRITERS 2
#define READS_PER_READER 5
#define WRITES_PER_WRITER 3

int shared_data = 0;
int readcount = 0;

sem_t mutex;   
sem_t rw_sem;  

void rand_sleep_ms(int min_ms, int max_ms) {
    int ms = min_ms + rand() % (max_ms - min_ms + 1);
    usleep(ms * 1000);
}

void *reader(void *arg) {
    int id = (int)(intptr_t)arg;
    for (int i = 0; i < READS_PER_READER; ++i) {
        sem_wait(&mutex);
        readcount++;
        if (readcount == 1) {
            sem_wait(&rw_sem);
        }
        sem_post(&mutex);

        printf("[Reader %d] starts reading. Shared data = %d (readcount=%d)\n", id, shared_data, readcount);
        rand_sleep_ms(50, 200);
        printf("[Reader %d] finished reading.\n", id);

        sem_wait(&mutex);
        readcount--;
        if (readcount == 0) {
            sem_post(&rw_sem);
        }
        sem_post(&mutex);

        // simulate time between reads
        rand_sleep_ms(100, 400);
    }
    printf("[Reader %d] done all reads and exiting.\n", id);
    return NULL;
}

void *writer(void *arg) {
    int id = (int)(intptr_t)arg;
    for (int i = 0; i < WRITES_PER_WRITER; ++i) {
    
        sem_wait(&rw_sem);

        shared_data += 10; 
        printf("    [Writer %d] starts writing. New shared data = %d\n", id, shared_data);
        rand_sleep_ms(100, 300); 
        printf("    [Writer %d] finished writing.\n", id);

        sem_post(&rw_sem);

        rand_sleep_ms(200, 500);
    }
    printf("    [Writer %d] done all writes and exiting.\n", id);
    return NULL;
}

int main(void) {
    srand((unsigned)time(NULL) ^ getpid());

    pthread_t readers[N_READERS];
    pthread_t writers[N_WRITERS];

    if (sem_init(&mutex, 0, 1) == -1) { perror("sem_init mutex"); exit(1); }
    if (sem_init(&rw_sem, 0, 1) == -1) { perror("sem_init rw_sem"); exit(1); }

    for (int i = 0; i < N_READERS; ++i) {
        if (pthread_create(&readers[i], NULL, reader, (void*)(intptr_t)(i+1)) != 0) {
            perror("pthread_create reader");
            exit(1);
        }
    }

    for (int i = 0; i < N_WRITERS; ++i) {
        if (pthread_create(&writers[i], NULL, writer, (void*)(intptr_t)(i+1)) != 0) {
            perror("pthread_create writer");
            exit(1);
        }
    }

    for (int i = 0; i < N_READERS; ++i) pthread_join(readers[i], NULL);
    for (int i = 0; i < N_WRITERS; ++i) pthread_join(writers[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&rw_sem);

    printf("All readers and writers have finished. Final shared data = %d\n", shared_data);
    return 0;
}

