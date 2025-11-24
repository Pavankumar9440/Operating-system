#include <stdio.h>
#include <stdlib.h>

int main() {
    int totalBlocks, indexBlock;
    int blocks[100] = {0};   // 0 = free, 1 = allocated
    int fileBlocks[50];      // stores block numbers pointed by index block
    int n, i, b;

    printf("=== Indexed File Allocation Simulation ===\n\n");

    printf("Enter total number of disk blocks (max 100): ");
    scanf("%d", &totalBlocks);

    printf("Enter the block number to be used as INDEX block: ");
    scanf("%d", &indexBlock);

    if (indexBlock < 0 || indexBlock >= totalBlocks) {
        printf("Invalid index block number!\n");
        return 0;
    }

    if (blocks[indexBlock] == 1) {
        printf("Block already allocated! Cannot use as index block.\n");
        return 0;
    }

    blocks[indexBlock] = 1;  // allocate index block

    printf("Enter number of blocks required for the file: ");
    scanf("%d", &n);

    printf("Enter the block numbers for the file:\n");
    for (i = 0; i < n; i++) {
        while (1) {
            printf("Block for file[%d]: ", i);
            scanf("%d", &b);

            if (b < 0 || b >= totalBlocks) {
                printf("Invalid block number! Enter again.\n");
            } else if (blocks[b] == 1) {
                printf("Block already allocated! Enter another block.\n");
            } else {
                blocks[b] = 1;        // allocate block
                fileBlocks[i] = b;    // store in index
                break;
            }
        }
    }

    // Display file allocation
    printf("\nFile allocated using Indexed Allocation:\n");
    printf("Index Block: %d\n", indexBlock);
    printf("Blocks pointed by Index Block:\n");
    for (i = 0; i < n; i++) {
        printf("  [%d] -> Block %d\n", i, fileBlocks[i]);
    }

    return 0;
}

