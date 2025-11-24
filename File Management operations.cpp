#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createFile() {
    char filename[256];
    FILE *fp;

    printf("Enter file name to create: ");
    scanf("%255s", filename);

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Could not create file.\n");
        return;
    }
    printf("File '%s' created successfully.\n", filename);
    fclose(fp);
}

void writeFile() {
    char filename[256];
    char text[1000];
    FILE *fp;

    printf("Enter file name to write into: ");
    scanf("%255s", filename);

    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    printf("Enter text to write (single line, no spaces at start):\n");
    getchar(); // clear newline from buffer
    fgets(text, sizeof(text), stdin);

    fputs(text, fp);
    printf("Data written successfully to '%s'.\n", filename);

    fclose(fp);
}

void appendFile() {
    char filename[256];
    char text[1000];
    FILE *fp;

    printf("Enter file name to append into: ");
    scanf("%255s", filename);

    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error: Could not open file for appending.\n");
        return;
    }

    printf("Enter text to append (single line):\n");
    getchar(); // clear newline
    fgets(text, sizeof(text), stdin);

    fputs(text, fp);
    printf("Data appended successfully to '%s'.\n", filename);

    fclose(fp);
}

void readFile() {
    char filename[256];
    FILE *fp;
    int ch;

    printf("Enter file name to read: ");
    scanf("%255s", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    printf("\n--- Contents of '%s' ---\n", filename);
    while ((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }
    printf("\n--- End of file ---\n");

    fclose(fp);
}

void copyFile() {
    char src[256], dest[256];
    FILE *fps, *fpd;
    int ch;

    printf("Enter source file name: ");
    scanf("%255s", src);

    printf("Enter destination file name: ");
    scanf("%255s", dest);

    fps = fopen(src, "r");
    if (fps == NULL) {
        printf("Error: Could not open source file.\n");
        return;
    }

    fpd = fopen(dest, "w");
    if (fpd == NULL) {
        printf("Error: Could not open/create destination file.\n");
        fclose(fps);
        return;
    }

    while ((ch = fgetc(fps)) != EOF) {
        fputc(ch, fpd);
    }

    printf("File '%s' copied to '%s' successfully.\n", src, dest);

    fclose(fps);
    fclose(fpd);
}

void renameFile() {
    char oldname[256], newname[256];

    printf("Enter current file name: ");
    scanf("%255s", oldname);

    printf("Enter new file name: ");
    scanf("%255s", newname);

    if (rename(oldname, newname) == 0) {
        printf("File renamed from '%s' to '%s' successfully.\n", oldname, newname);
    } else {
        printf("Error: Could not rename file.\n");
    }
}

void deleteFile() {
    char filename[256];

    printf("Enter file name to delete: ");
    scanf("%255s", filename);

    if (remove(filename) == 0) {
        printf("File '%s' deleted successfully.\n", filename);
    } else {
        printf("Error: Could not delete file.\n");
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n==== FILE MANAGEMENT OPERATIONS ====\n");
        printf("1. Create file\n");
        printf("2. Write to file\n");
        printf("3. Append to file\n");
        printf("4. Read file\n");
        printf("5. Copy file\n");
        printf("6. Rename file\n");
        printf("7. Delete file\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            createFile();
            break;
        case 2:
            writeFile();
            break;
        case 3:
            appendFile();
            break;
        case 4:
            readFile();
            break;
        case 5:
            copyFile();
            break;
        case 6:
            renameFile();
            break;
        case 7:
            deleteFile();
            break;
        case 8:
            printf("Exiting program.\n");
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

