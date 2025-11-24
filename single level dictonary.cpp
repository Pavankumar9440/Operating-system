#include <stdio.h>
#include <string.h>

#define MAX 50

int main() {
    char dir[MAX][30];
    int n = 0;  
    int choice;
    char name[30];
    int i, found;

    while (1) {
        printf("\n=== SINGLE LEVEL DIRECTORY ===\n");
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Search File\n");
        printf("4. Display Files\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter file name to create: ");
            scanf("%s", name);

           
            found = 0;
            for (i = 0; i < n; i++) {
                if (strcmp(dir[i], name) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found)
                printf("File already exists!\n");
            else {
                strcpy(dir[n], name);
                n++;
                printf("File created successfully.\n");
            }
        }

        else if (choice == 2) {
            printf("Enter file name to delete: ");
            scanf("%s", name);

            found = -1;
            for (i = 0; i < n; i++) {
                if (strcmp(dir[i], name) == 0) {
                    found = i;
                    break;
                }
            }

            if (found == -1)
                printf("File not found!\n");
            else {
                for (i = found; i < n - 1; i++)
                    strcpy(dir[i], dir[i + 1]);

                n--;
                printf("File deleted successfully.\n");
            }
        }

        else if (choice == 3) {
            printf("Enter file name to search: ");
            scanf("%s", name);

            found = 0;
            for (i = 0; i < n; i++) {
                if (strcmp(dir[i], name) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found)
                printf("File found in directory.\n");
            else
                printf("File not found.\n");
        }

        else if (choice == 4) {
            printf("\nFiles in Directory:\n");
            if (n == 0)
                printf("No files available.\n");
            else {
                for (i = 0; i < n; i++)
                    printf("%s\n", dir[i]);
            }
        }

        else if (choice == 5) {
            printf("Exiting...\n");
            break;
        }

        else {
            printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

