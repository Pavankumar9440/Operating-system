#include <stdio.h>

int main() {
    int frames[10], pages[30];
    int nFrames, nPages;
    int i, j, k;
    int pageFaults = 0, pageHits = 0;
    int front = 0;   // FIFO pointer (oldest frame index)
    int found;

    printf("=== FIFO Page Replacement Simulation ===\n\n");

    printf("Enter number of frames (max 10): ");
    scanf("%d", &nFrames);

    printf("Enter number of pages in reference string (max 30): ");
    scanf("%d", &nPages);

    printf("Enter the reference string (page numbers):\n");
    for (i = 0; i < nPages; i++) {
        scanf("%d", &pages[i]);
    }

    // Initialize all frames to -1 (empty)
    for (i = 0; i < nFrames; i++) {
        frames[i] = -1;
    }

    printf("\nPage\tFrames\t\t\tRemark\n");
    printf("-------------------------------------------------\n");

    for (i = 0; i < nPages; i++) {
        found = 0;

        // Check if page is already in any frame (HIT?)
        for (j = 0; j < nFrames; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
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
            // Page Fault: replace page using FIFO
            frames[front] = pages[i];
            front = (front + 1) % nFrames; // move to next frame
            pageFaults++;

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

