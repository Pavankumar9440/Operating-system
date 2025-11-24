#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

void write_shared_memory() {
    FILE *fp;
    char data[100];

    fp = fopen("shared_memory.txt", "w");
    if (!fp) {
        printf("Error creating shared memory file!\n");
        return;
    }

    printf("Enter data to write into shared memory: ");
    fgets(data, sizeof(data), stdin);
    trim_newline(data);

    fprintf(fp, "%s\n", data);
    fclose(fp);

    printf("Data written successfully.\n");
}

void read_shared_memory() {
    FILE *fp;
    char buffer[100];

    fp = fopen("shared_memory.txt", "r");
    if (!fp) {
        printf("Shared memory not found! Please write first.\n");
        return;
    }

    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        trim_newline(buffer);
        printf("Data read from shared memory: %s\n", buffer);
    } else {
        printf("Shared memory is empty.\n");
    }

    fclose(fp);
}

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int main() {
    int choice;

    while (1) {
        printf("\n=== SHARED MEMORY IPC SIMULATION ===\n");
        printf("1. Write to Shared Memory\n");
        printf("2. Read from Shared Memory\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Try again.\n");
            clear_stdin();
            continue;
        }

        clear_stdin();  
        if (choice == 1) {
            write_shared_memory();
        } else if (choice == 2) {
            read_shared_memory();
        } else if (choice == 3) {
            printf("Exiting...\n");
            return 0;
        } else {
            printf("Invalid choice! Try again.\n");
        }
    }
}

