#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REC 50
#define MAX_LEN 50

struct Record {
    int id;               // record number
    char data[MAX_LEN];   // record data
};

int main() {
    struct Record file[MAX_REC];
    int n, i, choice, k;

    printf("=== Simulation of Sequential File Allocation ===\n\n");

    printf("Enter number of records in the file (max %d): ", MAX_REC);
    scanf("%d", &n);

    printf("Enter data for each record:\n");
    for (i = 0; i < n; i++) {
        file[i].id = i + 1;
        printf("Record %d data: ", file[i].id);
        scanf("%s", file[i].data);   // simple word data (no spaces)
    }

    while (1) {
        printf("\nMenu:\n");
        printf("1. Display all records (sequential)\n");
        printf("2. Access a specific record (simulate sequential access)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("\nAll records in the file (stored sequentially):\n");
            for (i = 0; i < n; i++) {
                printf("Record %d: %s\n", file[i].id, file[i].data);
            }
            break;

        case 2:
            printf("Enter record number to access (1 to %d): ", n);
            scanf("%d", &k);
            if (k < 1 || k > n) {
                printf("Invalid record number!\n");
            } else {
                printf("\nSimulating sequential access to record %d:\n", k);
                for (i = 0; i < k; i++) {
                    printf("Reading Record %d: %s\n", file[i].id, file[i].data);
                }
                printf("=> Record %d accessed after reading all previous records.\n", k);
            }
            break;

        case 3:
            printf("Exiting...\n");
            exit(0);

        default:
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

