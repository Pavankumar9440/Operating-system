/* bankers_fixed.c
   Banker's algorithm simulation (works with C and C++ compilers)
   Compile (C):  gcc -std=c11 bankers_fixed.c -o bankers
   Compile (C++): g++ bankers_fixed.c -o bankers
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void print_matrix(const char *name, int **mat, int n, int m) {
    printf("%s:\n", name);
    for (int i = 0; i < n; ++i) {
        printf("P%d: ", i);
        for (int j = 0; j < m; ++j) printf("%d ", mat[i][j]);
        printf("\n");
    }
}

void print_vector(const char *name, int *v, int m) {
    printf("%s: ", name);
    for (int j = 0; j < m; ++j) printf("%d ", v[j]);
    printf("\n");
}

/* returns true if Need[i] <= Work for all resource types */
bool need_leq_work(int *need_i, int *work, int m) {
    for (int j = 0; j < m; ++j) if (need_i[j] > work[j]) return false;
    return true;
}

/* safety check: if safe, fills safe_seq (size n) and returns true */
bool is_safe_state(int n, int m, int *available, int **allocation, int **need, int *safe_seq_out) {
    int *work = (int*) malloc(sizeof(int) * m);
    bool *finish = (bool*) malloc(sizeof(bool) * n);
    if (!work || !finish) {
        perror("malloc");
        free(work); free(finish);
        return false;
    }
    for (int j = 0; j < m; ++j) work[j] = available[j];
    for (int i = 0; i < n; ++i) finish[i] = false;

    int count = 0;
    while (count < n) {
        bool progressed = false;
        for (int i = 0; i < n; ++i) {
            if (!finish[i] && need_leq_work(need[i], work, m)) {
                // simulate allocation release
                for (int j = 0; j < m; ++j) work[j] += allocation[i][j];
                finish[i] = true;
                safe_seq_out[count++] = i;
                progressed = true;
            }
        }
        if (!progressed) break;
    }

    bool safe = (count == n);
    free(work);
    free(finish);
    return safe;
}

int main(void) {
    int n, m;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 0;
    printf("Enter number of resource types: ");
    if (scanf("%d", &m) != 1 || m <= 0) return 0;

    int *available = (int*) malloc(sizeof(int) * m);
    if (!available) { perror("malloc"); return 1; }
    printf("Enter Available vector (m values):\n");
    for (int j = 0; j < m; ++j) scanf("%d", &available[j]);

    int **allocation = (int**) malloc(sizeof(int*) * n);
    int **max = (int**) malloc(sizeof(int*) * n);
    int **need = (int**) malloc(sizeof(int*) * n);
    if (!allocation || !max || !need) { perror("malloc"); free(available); free(allocation); free(max); free(need); return 1; }

    for (int i = 0; i < n; ++i) {
        allocation[i] = (int*) malloc(sizeof(int) * m);
        max[i] = (int*) malloc(sizeof(int) * m);
        need[i] = (int*) malloc(sizeof(int) * m);
        if (!allocation[i] || !max[i] || !need[i]) {
            perror("malloc");
            // free what we allocated so far
            for (int k = 0; k <= i; ++k) {
                if (allocation[k]) free(allocation[k]);
                if (max[k]) free(max[k]);
                if (need[k]) free(need[k]);
            }
            free(allocation); free(max); free(need); free(available);
            return 1;
        }
    }

    printf("Enter Allocation matrix (n rows, each with m values):\n");
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) scanf("%d", &allocation[i][j]);

    printf("Enter Max matrix (n rows, each with m values):\n");
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) {
            scanf("%d", &max[i][j]);
            need[i][j] = max[i][j] - allocation[i][j];
            if (need[i][j] < 0) need[i][j] = 0; // safety
        }

    print_vector("Available", available, m);
    print_matrix("Allocation", allocation, n, m);
    print_matrix("Max", max, n, m);
    print_matrix("Need", need, n, m);

    int *safe_seq = (int*) malloc(sizeof(int) * n);
    if (!safe_seq) { perror("malloc"); goto cleanup; }

    if (is_safe_state(n, m, available, allocation, need, safe_seq)) {
        printf("\nSystem is in a SAFE state.\nSafe sequence: ");
        for (int i = 0; i < n; ++i) {
            printf("P%d", safe_seq[i]);
            if (i < n-1) printf(" -> ");
        }
        printf("\n");
    } else {
        printf("\nSystem is in an UNSAFE state (no safe sequence).\n");
    }

    /* Optional: simulate a request */
    char ans;
    printf("\nDo you want to simulate a request? (y/n): ");
    scanf(" %c", &ans);
    if (ans == 'y' || ans == 'Y') {
        int p;
        printf("Enter requesting process index (0..%d): ", n-1);
        scanf("%d", &p);
        if (p < 0 || p >= n) { printf("Invalid process.\n"); goto cleanup; }
        int *request = (int*) malloc(sizeof(int) * m);
        if (!request) { perror("malloc"); goto cleanup; }
        printf("Enter request vector (m values):\n");
        for (int j = 0; j < m; ++j) scanf("%d", &request[j]);

        bool ok = true;
        for (int j = 0; j < m; ++j) if (request[j] > need[p][j]) ok = false;
        if (!ok) { printf("Error: process has exceeded its maximum claim.\n"); free(request); goto cleanup; }

        ok = true;
        for (int j = 0; j < m; ++j) if (request[j] > available[j]) ok = false;
        if (!ok) { printf("Request cannot be granted now (not enough available resources).\n"); free(request); goto cleanup; }

        /* Tentatively allocate */
        for (int j = 0; j < m; ++j) { available[j] -= request[j]; allocation[p][j] += request[j]; need[p][j] -= request[j]; }

        if (is_safe_state(n, m, available, allocation, need, safe_seq)) {
            printf("Request can be GRANTED. New safe sequence: ");
            for (int i = 0; i < n; ++i) {
                printf("P%d", safe_seq[i]);
                if (i < n-1) printf(" -> ");
            }
            printf("\n");
        } else {
            printf("Request CANNOT be granted — it leads to unsafe state. Rolling back.\n");
            /* rollback */
            for (int j = 0; j < m; ++j) { available[j] += request[j]; allocation[p][j] -= request[j]; need[p][j] += request[j]; }
        }
        free(request);
    }

cleanup:
    for (int i = 0; i < n; ++i) {
        if (allocation[i]) free(allocation[i]);
        if (max[i]) free(max[i]);
        if (need[i]) free(need[i]);
    }
    if (allocation) free(allocation);
    if (max) free(max);
    if (need) free(need);
    if (available) free(available);
    if (safe_seq) free(safe_seq);
    return 0;
}

