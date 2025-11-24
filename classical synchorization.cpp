#include <stdio.h>
#include <stdlib.h>

int mutex = 1, full = 0, empty = 5, x = 0;

int wait(int s) { return (--s); }
int signal(int s) { return (++s); }

void producer() {
    mutex = wait(mutex);
    full = signal(full);
    empty = wait(empty);
    x++;
    printf("Producer produced item %d\n", x);
    mutex = signal(mutex);
}

void consumer() {
    mutex = wait(mutex);
    full = wait(full);
    empty = signal(empty);
    printf("Consumer consumed item %d\n", x);
    x--;
    mutex = signal(mutex);
}

int main() {
    int ch;
    printf("=== Producer-Consumer Synchronization ===\n");

    while (1) {
        printf("\n1. Produce\n2. Consume\n3. Exit\nEnter choice: ");
        scanf("%d", &ch);

        switch (ch) {
        case 1:
            if (mutex == 1 && empty != 0) producer();
            else printf("Buffer full! Cannot produce.\n");
            break;
        case 2:
            if (mutex == 1 && full != 0) consumer();
            else printf("Buffer empty! Cannot consume.\n");
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice.\n");
        }
    }
    return 0;
}

