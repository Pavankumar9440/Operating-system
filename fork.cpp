#include <stdio.h>
#include <unistd.h>     // for fork(), getpid(), getppid()

int main() {
    pid_t pid;

    // Create new process
    pid = fork();

    if (pid < 0) {
        // Fork failed
        printf("Process creation failed.\n");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child Process:\n");
        printf("Child PID      : %d\n", getpid());
        printf("Parent PID     : %d\n", getppid());
    }
    else {
        // Parent process
        printf("Parent Process:\n");
        printf("Parent PID     : %d\n", getpid());
        printf("Parent's Parent PID : %d\n", getppid());
    }

    return 0;
}
