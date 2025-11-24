#include <stdio.h>

int main() {
    int frames[10], pages[30];
    int nFrames, nPages;
    int i, j, k;
    int pageFaults = 0, pageHits = 0;
    int found, pos;
    int nextUse[10], farthest;

    printf("=== Optimal Page Replacement Simulation ===\n\n");

    printf("Enter number of frames (max 10): ");
    scanf("%d", &nFrames);

    printf("Enter number of pages in reference string (max 30): ");
    scanf("%d", &nPages);

    printf("Enter the reference string (page numbers):\n");
    for (i = 0; i < nPages; i++) {
        scanf("%d", &pages[i]);
    }

    for (i = 0; i < nFrames; i++) {
        frames[i] = -1;
    }

    printf("\nPage\tFrames\t\t\tRemark\n");
    printf("-------------------------------------------------\n");

    for (i = 0; i < nPages; i++) {
        found = 0;

        for (j = 0; j < nFrames; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        printf("%d\t", pages[i]);

        if (found == 1) {
            pageHits++;
            for (k = 0; k < nFrames; k++)
                frames[k] == -1 ? printf("- ") : printf("%d ", frames[k]);
            printf("\tHit\n");
        } else {
            pageFaults++;
            pos = -1;

            for (j = 0; j < nFrames; j++) {
                if (frames[j] == -1) {
                    pos = j;
                    break;
                }
            }

            if (pos == -1) {
                for (j = 0; j < nFrames; j++) {
                    nextUse[j] = -1;
                    for (k = i + 1; k < nPages; k++) {
                        if (pages[k] == frames[j]) {
                            nextUse[j] = k;
                            break;
                        }
                    }
                }

                farthest = -1;
                pos = 0;
                for (j = 0; j < nFrames; j++) {
                    if (nextUse[j] == -1) {
                        pos = j;
                        break;
                    }
                    if (nextUse[j] > farthest) {
                        farthest = nextUse[j];
                        pos = j;
                    }
                }
            }

            frames[pos] = pages[i];
            for (k = 0; k < nFrames; k++)
                frames[k] == -1 ? printf("- ") : printf("%d ", frames[k]);
            printf("\tFault\n");
        }
    }

    printf("-------------------------------------------------\n");
    printf("Total Page Faults = %d\n", pageFaults);
    printf("Total Page Hits   = %d\n", pageHits);

    return 0;
}

