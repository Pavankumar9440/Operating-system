#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_shared_memory(void) {
    FILE *fp;
    char data[100];

    fp = fopen("shared_memory.txt", "w");
    if (!fp) {
        printf("Error creating shared memory file!\n");
        return;
    }

    printf("Enter data to write into shared memory: ");
    if (fgets(data, (int)sizeof(data), stdin) == NULL) {
        printf("No input received.\n");
        fclose(fp);
        return;
    }

    size_t len = strlen(data);
    if (len > 0 && data[len-1] == '\n') data[len-1] = '\0';

    fprintf(fp, "%s\n", data);
    fclose(fp);

    printf("Data written successfully.\n");
}

void read_shared_memory(void) {
    FILE *fp;
    char buffer[100];

    fp = fopen("shared_memory.txt", "r");
    if (!fp) {
        printf("Shared memory not found! Please write first.\n");
        return;
    }

    if (fgets(buffer, (int)sizeof(buffer), fp) == NULL) {
        printf("Shared memory is empty.\n");
        fclose(fp);
        return;
    }
    fclose(fp);

  
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';

    printf("Data read from shared memory: %s\n", buffer);
}

int main(void) {
    int choice;

    while (1) {
        printf("\n=== SHARED MEMORY IPC SIMULATION ===\n");
        printf("1. Write to Shared Memory\n");
        printf("2. Read from Shared Memory\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
           
            int c;
            while ((c = getchar()) != '\n' && c != EOF) ;
            printf("Invalid input. Try again.\n");
            continue;
        }
   
        getchar();

        if (choice == 1) {
            write_shared_memory();
        }
        else if (choice == 2) {
            read_shared_memory();
        }
        else if (choice == 3) {
            exit(0);
        }
        else {
            printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}

