#include <stdio.h>
#include <stdlib.h>

int main() {
    int totalBlocks;
    int allocated[100] = {0};   // 0 = free, 1 = allocated
    int next[100];              // next pointer for each block
    int start = -1, end = -1;   // start and end blocks of the file
    int n, i, b;

    printf("=== Linked File Allocation Simulation ===\n\n");

    printf("Enter total number of disk blocks (max 100): ");
    scanf("%d", &totalBlocks);

    printf("Enter number of blocks required for the file: ");
    scanf("%d", &n);

    if (n > totalBlocks) {
        printf("Not enough blocks available!\n");
        return 0;
    }

    printf("\nEnter the block numbers to allocate for the file:\n");

    for (i = 0; i < n; i++) {
        while (1) {
            printf("Block for file node %d: ", i + 1);
            scanf("%d", &b);

            if (b < 0 || b >= totalBlocks) {
                printf("Invalid block number! Enter again.\n");
            } else if (allocated[b] == 1) {
                printf("Block already allocated! Choose another.\n");
            } else {
                // allocate this block
                allocated[b] = 1;
                next[b] = -1;   // currently end of list

                if (start == -1) {
                    // first block of file
                    start = b;
                    end = b;
                } else {
                    // link from previous end to this block
                    next[end] = b;
                    end = b;
                }
                break;
            }
        }
    }

    // Display allocation info
    printf("\nFile allocated using Linked Allocation:\n");
    printf("Start Block: %d\n", start);
    printf("End Block  : %d\n", end);

    printf("\nLinked list of blocks for the file:\n");
    printf("%d", start);
    int cur = next[start];
    while (cur != -1) {
        printf(" -> %d", cur);
        cur = next[cur];
    }
    printf(" -> NULL\n");

    return 0;
}

