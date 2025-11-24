#include <stdio.h>

int main() {
    int frames[10], pages[30], lastUsed[10];
    int nFrames, nPages;
    int i, j, k;
    int pageFaults = 0, pageHits = 0;
    int found, pos, min;

    printf("=== LRU Page Replacement Simulation ===\n\n");

    printf("Enter number of frames (max 10): ");
    scanf("%d", &nFrames);

    printf("Enter number of pages in reference string (max 30): ");
    scanf("%d", &nPages);

    printf("Enter the reference string (page numbers):\n");
    for (i = 0; i < nPages; i++) {
        scanf("%d", &pages[i]);
    }

    // Initialize frames and lastUsed info
    for (i = 0; i < nFrames; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }

    printf("\nPage\tFrames\t\t\tRemark\n");
    printf("-------------------------------------------------\n");

    for (i = 0; i < nPages; i++) {
        found = 0;

        // Check if page is already in any frame (HIT?)
        for (j = 0; j < nFrames; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                lastUsed[j] = i;   // update last used time
                break;
            }
        }

        printf("%d\t", pages[i]);

        if (found == 1) {
            // Page Hit
            pageHits++;
            for (k = 0; k < nFrames; k++) {
                if (frames[k] == -1)
                    printf("- ");
                else
                    printf("%d ", frames[k]);
            }
            printf("\tHit\n");
        } else {
            // Page Fault
            pageFaults++;

            // Check for empty frame first
            pos = -1;
            for (j = 0; j < nFrames; j++) {
                if (frames[j] == -1) {
                    pos = j;
                    break;
                }
            }

            // If no empty frame, find LRU frame
            if (pos == -1) {
                min = lastUsed[0];
                pos = 0;
                for (j = 1; j < nFrames; j++) {
                    if (lastUsed[j] < min) {
                        min = lastUsed[j];
                        pos = j;
                    }
                }
            }

            // Replace page at position pos
            frames[pos] = pages[i];
            lastUsed[pos] = i;

            for (k = 0; k < nFrames; k++) {
                if (frames[k] == -1)
                    printf("- ");
                else
                    printf("%d ", frames[k]);
            }
            printf("\tFault\n");
        }
    }

    printf("-------------------------------------------------\n");
    printf("Total Page Faults = %d\n", pageFaults);
    printf("Total Page Hits   = %d\n", pageHits);

    return 0;
}

