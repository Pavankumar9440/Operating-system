#include <stdio.h>
#include <sys/stat.h>

int main() {
    struct stat st;
    char filename[100];

    printf("Enter filename: ");
    scanf("%s", filename);

    if (stat(filename, &st) == -1) {
        perror("stat");
        return 1;
    }

    printf("\nPermissions for file: %s\n", filename);

    printf("File attributes (Windows format):\n");
    printf("Read only: %s\n", (st.st_mode & S_IWRITE) ? "No" : "Yes");
    printf("Writable: %s\n", (st.st_mode & S_IWRITE) ? "Yes" : "No");

    return 0;
}

