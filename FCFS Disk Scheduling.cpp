#include <stdio.h>
#include <stdlib.h>

int main() {
    int requests[50];
    int n, i;
    int head, total_movement = 0, movement;

    printf("=== FCFS Disk Scheduling Simulation ===\n\n");

    printf("Enter number of disk requests (max 50): ");
    scanf("%d", &n);

    printf("Enter the disk request sequence (track numbers):\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("\nOrder of servicing requests (FCFS):\n");
    printf("Head starts at: %d\n", head);

    for (i = 0; i < n; i++) {
        movement = abs(requests[i] - head);
        printf("Move from %d to %d  (movement = %d)\n",
               head, requests[i], movement);
        total_movement += movement;
        head = requests[i];
    }

    printf("\nTotal head movement = %d tracks\n", total_movement);

    return 0;
}

