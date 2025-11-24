#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "employee.dat"
#define MAX_RECORDS 100

typedef struct {
    int id;                 
    char name[30];
    int age;
    float salary;
    char dept[20];
} Employee;

static void trim_newline(char *s) {
    if (s == NULL) return;
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len - 1] == '\n') s[len - 1] = '\0';
}

void init_file_if_needed() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) {
        fp = fopen(FILENAME, "wb");
        if (!fp) { perror("fopen"); exit(1); }
        Employee e;
        memset(&e, 0, sizeof(Employee));
        for (int i = 0; i < MAX_RECORDS; ++i) fwrite(&e, sizeof(Employee), 1, fp);
    }
    if (fp) fclose(fp);
}

void write_record(const Employee *e) {
    if (e->id < 1 || e->id > MAX_RECORDS) {
        printf("ID must be between 1 and %d\n", MAX_RECORDS);
        return;
    }
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) { perror("fopen"); return; }
    if (fseek(fp, (long)(e->id - 1) * sizeof(Employee), SEEK_SET) != 0) {
        perror("fseek");
        fclose(fp);
        return;
    }
    if (fwrite(e, sizeof(Employee), 1, fp) != 1) {
        perror("fwrite");
    } else {
        printf("Record written for ID %d\n", e->id);
    }
    fclose(fp);
}

int read_record(int id, Employee *e_out) {
    if (id < 1 || id > MAX_RECORDS) return 0;
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { perror("fopen"); return 0; }
    if (fseek(fp, (long)(id - 1) * sizeof(Employee), SEEK_SET) != 0) {
        perror("fseek");
        fclose(fp);
        return 0;
    }
    if (fread(e_out, sizeof(Employee), 1, fp) != 1) { fclose(fp); return 0; }
    fclose(fp);
    if (e_out->id == 0) return 0; 
    return 1;
}

void input_employee(Employee *e) {
    printf("Enter ID (1..%d): ", MAX_RECORDS);
    while (scanf("%d", &e->id) != 1 || e->id < 1 || e->id > MAX_RECORDS) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        printf("Invalid ID. Enter ID (1..%d): ", MAX_RECORDS);
    }
    getchar(); /* consume newline */

    printf("Enter name: ");
    if (fgets(e->name, sizeof e->name, stdin) == NULL) e->name[0] = '\0';
    trim_newline(e->name);

    printf("Enter age: ");
    while (scanf("%d", &e->age) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF) { }
        printf("Invalid age. Enter age: ");
    }

    printf("Enter salary: ");
    while (scanf("%f", &e->salary) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF) { }
        printf("Invalid salary. Enter salary: ");
    }
    getchar(); 

    printf("Enter department: ");
    if (fgets(e->dept, sizeof e->dept, stdin) == NULL) e->dept[0] = '\0';
    trim_newline(e->dept);
}

void add_record() {
    Employee e;
    memset(&e, 0, sizeof e);
    input_employee(&e);

    Employee tmp;
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { perror("fopen"); return; }
    if (fseek(fp, (long)(e.id - 1) * sizeof(Employee), SEEK_SET) != 0) { perror("fseek"); fclose(fp); return; }
    fread(&tmp, sizeof(Employee), 1, fp);
    fclose(fp);

    if (tmp.id != 0) {
        printf("A record already exists at ID %d. Use modify to change it.\n", e.id);
        return;
    }
    write_record(&e);
}

void display_all() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { perror("fopen"); return; }
    Employee e;
    printf("\n%-4s %-20s %-4s %-10s %-10s\n", "ID", "Name", "Age", "Salary", "Dept");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < MAX_RECORDS; ++i) {
        if (fread(&e, sizeof(Employee), 1, fp) != 1) break;
        if (e.id != 0) {
            printf("%-4d %-20s %-4d %-10.2f %-10s\n", e.id, e.name, e.age, e.salary, e.dept);
        }
    }
    fclose(fp);
}

void display_by_id() {
    int id;
    printf("Enter ID to display: ");
    if (scanf("%d", &id) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid input\n"); return; }
    Employee e;
    if (read_record(id, &e)) {
        printf("\nID: %d\nName: %s\nAge: %d\nSalary: %.2f\nDepartment: %s\n",
               e.id, e.name, e.age, e.salary, e.dept);
    } else {
        printf("Record not found for ID %d\n", id);
    }
}

void modify_record() {
    int id;
    printf("Enter ID to modify: ");
    if (scanf("%d", &id) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid input\n"); return; }
    Employee e;
    if (!read_record(id, &e)) {
        printf("Record not found for ID %d\n", id);
        return;
    }
    printf("Current details:\nName: %s\nAge: %d\nSalary: %.2f\nDept: %s\n",
           e.name, e.age, e.salary, e.dept);
    getchar(); 

    printf("Enter new name (leave empty to keep): ");
    char buffer[100];
    if (fgets(buffer, sizeof buffer, stdin) != NULL) {
        trim_newline(buffer);
        if (buffer[0] != '\0') strncpy(e.name, buffer, sizeof e.name - 1);
    }

    printf("Enter new age (0 to keep): ");
    int age;
    if (scanf("%d", &age) == 1) {
        if (age > 0) e.age = age;
    } else {
        int c; while ((c = getchar()) != '\n' && c != EOF);
    }

    printf("Enter new salary (0 to keep): ");
    float sal;
    if (scanf("%f", &sal) == 1) {
        if (sal > 0.0f) e.salary = sal;
    } else {
        int c; while ((c = getchar()) != '\n' && c != EOF);
    }
    getchar(); 

    printf("Enter new department (leave empty to keep): ");
    if (fgets(buffer, sizeof buffer, stdin) != NULL) {
        trim_newline(buffer);
        if (buffer[0] != '\0') strncpy(e.dept, buffer, sizeof e.dept - 1);
    }

    write_record(&e);
    printf("Record updated.\n");
}

void delete_record() {
    int id;
    printf("Enter ID to delete: ");
    if (scanf("%d", &id) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); printf("Invalid input\n"); return; }
    Employee e;
    if (!read_record(id, &e)) {
        printf("Record not found for ID %d\n", id);
        return;
    }
    Employee empty;
    memset(&empty, 0, sizeof empty);
    empty.id = 0;
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) { perror("fopen"); return; }
    if (fseek(fp, (long)(id - 1) * sizeof(Employee), SEEK_SET) != 0) { perror("fseek"); fclose(fp); return; }
    if (fwrite(&empty, sizeof(Employee), 1, fp) != 1) perror("fwrite");
    fclose(fp);
    printf("Record with ID %d deleted.\n", id);
}

int main(void) {
    init_file_if_needed();
    int choice;
    while (1) {
        printf("\n=== Random Access Employee Database ===\n");
        printf("1. Add new record\n");
        printf("2. Display all records\n");
        printf("3. Display record by ID\n");
        printf("4. Modify record by ID\n");
        printf("5. Delete record by ID\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { printf("Invalid input\n"); exit(1); }
        switch (choice) {
            case 1: add_record(); break;
            case 2: display_all(); break;
            case 3: display_by_id(); break;
            case 4: modify_record(); break;
            case 5: delete_record(); break;
            case 6: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice\n");
        }
    }
    return 0;
}

