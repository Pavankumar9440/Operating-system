#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

int main() {
    key_t key = ftok("shmfile", 65);    
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    pid_t pid = fork();

    if (pid > 0) {
    
        char *str = (char*) shmat(shmid, NULL, 0);

        printf("Parent: Enter message to write in shared memory:\n");
        fgets(str, 100, stdin);

        printf("Parent wrote: %s", str);

        shmdt(str);
        wait(NULL);
    }
    else {
        
        sleep(1); 
        char *str = (char*) shmat(shmid, NULL, 0);

        printf("\nChild: Reading message from shared memory...\n");
        printf("Child read: %s", str);

        shmdt(str);
        shmctl(shmid, IPC_RMID, NULL); 
    }

    return 0;
}

