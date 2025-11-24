/* prodcons_win.c
   Producer-Consumer using Win32 semaphores and threads.
   Compile (MinGW):  gcc prodcons_win.c -o prodcons_win.exe -lws2_32
   Compile (MSVC):   cl prodcons_win.c
   Run: prodcons_win.exe
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 20

/* shared buffer and indices */
int buffer[BUFFER_SIZE];
int in_idx = 0;
int out_idx = 0;
int produced = 0;
int consumed = 0;

/* synchronization objects */
HANDLE semEmpty = NULL;   // counts empty slots
HANDLE semFull  = NULL;   // counts filled slots
HANDLE semMutex = NULL;   // binary semaphore as mutex

DWORD WINAPI ProducerThread(LPVOID lpParam) {
    int id = GetCurrentThreadId();
    srand((unsigned)time(NULL) ^ id);
    while (1) {
        WaitForSingleObject(semEmpty, INFINITE);
        WaitForSingleObject(semMutex, INFINITE);

        if (produced >= PRODUCE_COUNT) {
            // rollback and exit
            ReleaseSemaphore(semMutex, 1, NULL);
            ReleaseSemaphore(semEmpty, 1, NULL);
            break;
        }

        produced++;
        int item = produced;
        buffer[in_idx] = item;
        printf("[Producer %lu] Produced item %d at index %d\n", (unsigned long)id, item, in_idx);
        in_idx = (in_idx + 1) % BUFFER_SIZE;

        ReleaseSemaphore(semMutex, 1, NULL);
        ReleaseSemaphore(semFull, 1, NULL);

        Sleep((rand() % 200) + 100); // 100-300 ms
    }
    printf("[Producer %lu] Exiting\n", (unsigned long)id);
    return 0;
}

DWORD WINAPI ConsumerThread(LPVOID lpParam) {
    int id = GetCurrentThreadId();
    srand((unsigned)time(NULL) ^ id);
    while (1) {
        WaitForSingleObject(semFull, INFINITE);
        WaitForSingleObject(semMutex, INFINITE);

        if (consumed >= PRODUCE_COUNT) {
            // rollback and exit
            ReleaseSemaphore(semMutex, 1, NULL);
            ReleaseSemaphore(semFull, 1, NULL);
            break;
        }

        int item = buffer[out_idx];
        consumed++;
        printf("    [Consumer %lu] Consumed item %d from index %d\n", (unsigned long)id, item, out_idx);
        out_idx = (out_idx + 1) % BUFFER_SIZE;

        ReleaseSemaphore(semMutex, 1, NULL);
        ReleaseSemaphore(semEmpty, 1, NULL);

        Sleep((rand() % 300) + 100); // 100-400 ms
    }
    printf("    [Consumer %lu] Exiting\n", (unsigned long)id);
    return 0;
}

int main(void) {
    /* initialize buffer and indices */
    for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = 0;
    in_idx = out_idx = produced = consumed = 0;

    /* create semaphores:
       semEmpty initial = BUFFER_SIZE (all slots empty),
       semFull  initial = 0,
       semMutex initial = 1 (binary semaphore)
    */
    semEmpty = CreateSemaphoreA(NULL, BUFFER_SIZE, BUFFER_SIZE, NULL);
    semFull  = CreateSemaphoreA(NULL, 0, BUFFER_SIZE, NULL);
    semMutex = CreateSemaphoreA(NULL, 1, 1, NULL);

    if (!semEmpty || !semFull || !semMutex) {
        fprintf(stderr, "CreateSemaphore failed: %lu\n", GetLastError());
        return 1;
    }

    /* create producer and consumer threads (you can create more) */
    HANDLE hProducer = CreateThread(NULL, 0, ProducerThread, NULL, 0, NULL);
    if (!hProducer) { fprintf(stderr, "CreateThread Producer failed: %lu\n", GetLastError()); return 1; }

    HANDLE hConsumer = CreateThread(NULL, 0, ConsumerThread, NULL, 0, NULL);
    if (!hConsumer) { fprintf(stderr, "CreateThread Consumer failed: %lu\n", GetLastError()); return 1; }

    /* wait for threads to finish */
    HANDLE handles[2] = { hProducer, hConsumer };
    WaitForMultipleObjects(2, handles, TRUE, INFINITE);

    /* cleanup */
    CloseHandle(hProducer);
    CloseHandle(hConsumer);
    CloseHandle(semEmpty);
    CloseHandle(semFull);
    CloseHandle(semMutex);

    printf("Parent: All threads exited. Program complete.\n");
    return 0;
}

