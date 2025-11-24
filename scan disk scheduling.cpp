#include <stdio.h>
#include <stdlib.h>

void sort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int requests[50], all[55];
    int n, i, j;
    int head, diskSize, dir;
    int total_movement = 0;
    int current, startIndex;

    printf("=== SCAN Disk Scheduling Simulation ===\n\n");

    printf("Enter number of disk requests (max 50): ");
    scanf("%d", &n);

    printf("Enter the disk request sequence (track numbers):\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size (total number of tracks, e.g., 200): ");
    scanf("%d", &diskSize);

    printf("Enter initial direction (1 = towards higher tracks, 0 = towards lower tracks): ");
    scanf("%d", &dir);

    /* Copy all requests and add boundary tracks 0 and diskSize-1 */
    for (i = 0; i < n; i++) {
        all[i] = requests[i];
    }
    all[n] = 0;
    all[n + 1] = diskSize - 1;
    int total = n + 2;

    /* Sort all track numbers */
    sort(all, total);

    /* Find the first index where all[i] >= head */
    startIndex = 0;
    for (i = 0; i < total; i++) {
        if (all[i] >= head) {
            startIndex = i;
            break;
        }
    }

    printf("\nOrder of head movement (SCAN):\n");
    current = head;
    printf("Head starts at: %d\n", current);

    if (dir == 1) {
        /* Move towards higher tracks first */
        for (i = startIndex; i < total; i++) {
            if (all[i] == current) continue;
            printf("Move from %d to %d  (movement = %d)\n",
                   current, all[i], abs(all[i] - current));
            total_movement += abs(all[i] - current);
            current = all[i];
        }
        /* Then reverse and move towards lower tracks */
        for (i = startIndex - 1; i >= 0; i--) {
            if (all[i] == current) continue;
            printf("Move from %d to %d  (movement = %d)\n",
                   current, all[i], abs(all[i] - current));
            total_movement += abs(all[i] - current);
            current = all[i];
        }
    } else {
        /* Move towards lower tracks first */
        for (i = startIndex - 1; i >= 0; i--) {
            if (all[i] == current) continue;
            printf("Move from %d to %d  (movement = %d)\n",
                   current, all[i], abs(all[i] - current));
            total_movement += abs(all[i] - current);
            current = all[i];
        }
        /* Then reverse and move towards higher tracks */
        for (i = startIndex; i < total; i++) {
            if (all[i] == current) continue;
            printf("Move from %d to %d  (movement = %d)\n",
                   current, all[i], abs(all[i] - current));
            total_movement += abs(all[i] - current);
            current = all[i];
        }
    }

    printf("\nTotal head movement = %d tracks\n", total_movement);

    return 0;
}

