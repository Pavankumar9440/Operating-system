#include <stdio.h>
#include <string.h>

#define MAX_USERS 20
#define MAX_FILES_PER_USER 50
#define MAX_NAME 40

int user_count = 0;
char users[MAX_USERS][MAX_NAME];
char files[MAX_USERS][MAX_FILES_PER_USER][MAX_NAME];
int file_count[MAX_USERS] = {0};

int find_user(const char *uname) {
    for (int i = 0; i < user_count; ++i)
        if (strcmp(users[i], uname) == 0) return i;
    return -1;
}

void create_user() {
    if (user_count >= MAX_USERS) {
        printf("Maximum users reached.\n");
        return;
    }
    char uname[MAX_NAME];
    printf("Enter user/directory name: ");
    scanf("%39s", uname);
    if (find_user(uname) != -1) {
        printf("User '%s' already exists.\n", uname);
        return;
    }
    strcpy(users[user_count], uname);
    file_count[user_count] = 0;
    user_count++;
    printf("User '%s' created.\n", uname);
}

void create_file() {
    char uname[MAX_NAME], fname[MAX_NAME];
    printf("Enter user/directory name: ");
    scanf("%39s", uname);
    int u = find_user(uname);
    if (u == -1) { printf("User '%s' not found.\n", uname); return; }
    if (file_count[u] >= MAX_FILES_PER_USER) { printf("Directory full for user '%s'.\n", uname); return; }
    printf("Enter file name to create: ");
    scanf("%39s", fname);
    for (int i = 0; i < file_count[u]; ++i)
        if (strcmp(files[u][i], fname) == 0) { printf("File already exists in '%s'.\n", uname); return; }
    strcpy(files[u][file_count[u]++], fname);
    printf("File '%s' created in '%s'.\n", fname, uname);
}

void delete_file() {
    char uname[MAX_NAME], fname[MAX_NAME];
    printf("Enter user/directory name: ");
    scanf("%39s", uname);
    int u = find_user(uname);
    if (u == -1) { printf("User '%s' not found.\n", uname); return; }
    printf("Enter file name to delete: ");
    scanf("%39s", fname);
    int idx = -1;
    for (int i = 0; i < file_count[u]; ++i) if (strcmp(files[u][i], fname) == 0) { idx = i; break; }
    if (idx == -1) { printf("File '%s' not found in '%s'.\n", fname, uname); return; }
    for (int i = idx; i < file_count[u] - 1; ++i) strcpy(files[u][i], files[u][i+1]);
    file_count[u]--;
    printf("File '%s' deleted from '%s'.\n", fname, uname);
}

void search_file() {
    char fname[MAX_NAME], uname[MAX_NAME];
    int choice;
    printf("Search in (1) specific user or (2) all users? Enter 1 or 2: ");
    scanf("%d", &choice);
    if (choice == 1) {
        printf("Enter user/directory name: ");
        scanf("%39s", uname);
        int u = find_user(uname);
        if (u == -1) { printf("User '%s' not found.\n", uname); return; }
        printf("Enter file name to search: ");
        scanf("%39s", fname);
        for (int i = 0; i < file_count[u]; ++i)
            if (strcmp(files[u][i], fname) == 0) { printf("File '%s' found in '%s'.\n", fname, uname); return; }
        printf("File '%s' not found in '%s'.\n", fname, uname);
    } else {
        printf("Enter file name to search: ");
        scanf("%39s", fname);
        int found = 0;
        for (int u = 0; u < user_count; ++u) {
            for (int i = 0; i < file_count[u]; ++i)
                if (strcmp(files[u][i], fname) == 0) {
                    printf("File '%s' found in user '%s'.\n", fname, users[u]);
                    found = 1;
                }
        }
        if (!found) printf("File '%s' not found in any user directory.\n", fname);
    }
}

void display_user_files() {
    char uname[MAX_NAME];
    printf("Enter user/directory name: ");
    scanf("%39s", uname);
    int u = find_user(uname);
    if (u == -1) { printf("User '%s' not found.\n", uname); return; }
    printf("Files in '%s':\n", uname);
    if (file_count[u] == 0) { printf("  (no files)\n"); return; }
    for (int i = 0; i < file_count[u]; ++i) printf("  %s\n", files[u][i]);
}

void display_all() {
    if (user_count == 0) { printf("No users/directories present.\n"); return; }
    printf("Two-level directory contents:\n");
    for (int u = 0; u < user_count; ++u) {
        printf("User: %s\n", users[u]);
        if (file_count[u] == 0) { printf("  (no files)\n"); continue; }
        for (int i = 0; i < file_count[u]; ++i) printf("  %s\n", files[u][i]);
    }
}

int main(void) {
    int choice;
    while (1) {
        printf("\n=== TWO-LEVEL DIRECTORY MENU ===\n");
        printf("1. Create User/Directory\n");
        printf("2. Create File in User Directory\n");
        printf("3. Delete File\n");
        printf("4. Search File\n");
        printf("5. Display Files of a User\n");
        printf("6. Display All Users and Files\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { 
            int c; while ((c = getchar()) != '\n' && c != EOF); 
            printf("Invalid input. Try again.\n"); 
            continue;
        }

        switch (choice) {
            case 1: create_user(); break;
            case 2: create_file(); break;
            case 3: delete_file(); break;
            case 4: search_file(); break;
            case 5: display_user_files(); break;
            case 6: display_all(); break;
            case 7: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}

