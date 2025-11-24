#include <stdio.h>

#define MAX 20

int main() {
    int nb, np;
    int blockSize[MAX], processSize[MAX];
    int alloc[MAX];   // alloc[i] = block index assigned to process i, or -1
    int i, j;

    printf("Enter number of memory blocks: ");
    scanf("%d", &nb);
    printf("Enter size of each block:\n");
    for (i = 0; i < nb; i++) {
        printf("Block %d size: ", i + 1);
        scanf("%d", &blockSize[i]);
    }

    printf("\nEnter number of processes: ");
    scanf("%d", &np);
    printf("Enter size of each process:\n");
    for (i = 0; i < np; i++) {
        printf("Process %d size: ", i + 1);
        scanf("%d", &processSize[i]);
        alloc[i] = -1;  // initially not allocated
    }

    // Worst Fit allocation
    for (i = 0; i < np; i++) {
        int worstIdx = -1;
        for (j = 0; j < nb; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j;
                }
            }
        }

        if (worstIdx != -1) {
            // allocate this process to block worstIdx
            alloc[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }

    printf("\nProcess No.\tProcess Size\tBlock No.\n");
    for (i = 0; i < np; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (alloc[i] != -1)
            printf("%d\n", alloc[i] + 1); // +1 for human-readable block number
        else
            printf("Not Allocated\n");
    }

    return 0;
}

