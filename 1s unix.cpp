#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>     // opendir, readdir, closedir
#include <string.h>     // strcmp

void list_directory(const char *path) {
    DIR *dp;
    struct dirent *entry;

    dp = opendir(path);
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    printf("Listing of directory: %s\n", path);
    printf("---------------------------------\n");

    while ((entry = readdir(dp)) != NULL) {
        /* Skip hidden files like . and .. and others starting with '.' */
        if (entry->d_name[0] == '.')
            continue;

        printf("%s\n", entry->d_name);
    }

    closedir(dp);
}

int main(int argc, char *argv[]) {
    const char *path;

    /* If no argument is given, use current directory like plain 'ls' */
    if (argc < 2) {
        path = ".";
    } else {
        path = argv[1];
    }

    list_directory(path);

    return 0;
}

