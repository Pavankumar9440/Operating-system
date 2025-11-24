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
    int n, i;
    int head, diskSize;
    int total_movement = 0;
    int current, startIndex, total;

    printf("=== C-SCAN Disk Scheduling Simulation ===\n\n");

    printf("Enter number of disk requests (max 50): ");
    scanf("%d", &n);

    printf("Enter the disk request sequence (track numbers):\n");
    for (i = 0; i < n; i++) {
        scanf("%d", &requests[i]);
    }

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter disk size (total number of tracks, e.g., 200): ");
    scanf("%d", &diskSize);   // tracks assumed 0 to diskSize-1

    /* Copy all requests and add 0 and max track (diskSize-1) */
    for (i = 0; i < n; i++) {
        all[i] = requests[i];
    }
    all[n] = 0;
    all[n + 1] = diskSize - 1;
    total = n + 2;

    /* Sort all track numbers */
    sort(all, total);

    /* Find first index where all[i] >= head */
    startIndex = 0;
    for (i = 0; i < total; i++) {
        if (all[i] >= head) {
            startIndex = i;
            break;
        }
    }

    printf("\nOrder of head movement (C-SCAN):\n");
    current = head;
    printf("Head starts at: %d\n", current);

    /* Move towards higher tracks: from head to diskSize-1 */
    for (i = startIndex; i < total; i++) {
        if (all[i] == current) continue;
        printf("Move from %d to %d  (movement = %d)\n",
               current, all[i], abs(all[i] - current));
        total_movement += abs(all[i] - current);
        current = all[i];
    }

    /* Jump from last track to 0 (circular scan) */
    if (current != 0) {
        printf("Move from %d to %d  (movement = %d)  [circular jump]\n",
               current, 0, current - 0);
        total_movement += current;   // since current is diskSize-1 typically
        current = 0;
    }

    /* Now move from 0 up to just before the starting index */
    for (i = 0; i < startIndex; i++) {
        if (all[i] == current) continue;
        printf("Move from %d to %d  (movement = %d)\n",
               current, all[i], abs(all[i] - current));
        total_movement += abs(all[i] - current);
        current = all[i];
    }

    printf("\nTotal head movement = %d tracks\n", total_movement);

    return 0;
}

