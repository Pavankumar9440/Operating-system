#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char filename[256];
    char pattern[100];
    char line[512];
    FILE *fp;
    int line_no = 0;
    int found = 0;

    printf("=== Simulation of GREP UNIX Command ===\n\n");

    printf("Enter pattern to search: ");
    scanf("%99s", pattern);      // Simple pattern (no spaces)

    printf("Enter file name: ");
    scanf("%255s", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_no++;
        // Check if pattern exists in this line
        if (strstr(line, pattern) != NULL) {
            if (!found) {
                printf("\nLines containing the pattern \"%s\":\n", pattern);
                printf("----------------------------------------\n");
            }
            found = 1;
            printf("Line %d: %s", line_no, line);
        }
    }

    if (!found) {
        printf("\nPattern \"%s\" not found in file %s\n", pattern, filename);
    }

    fclose(fp);
    return 0;
}

