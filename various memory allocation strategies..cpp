/*
 mem_alloc_strategies.c
 Compile: gcc mem_alloc_strategies.c -o mem_alloc_strategies
 Run: ./mem_alloc_strategies

 Simulates First Fit, Best Fit and Worst Fit allocation strategies.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 50
#define MAX_PID_LEN 16

typedef struct {
    int size;       // block size
    int is_free;    // 1 = free, 0 = occupied
    char pid[MAX_PID_LEN]; // process id occupying this block (empty if free)
} Block;

Block mem[MAX_BLOCKS];
int block_count = 0;

// Utility: print memory map
void print_memory() {
    printf("\nCurrent memory layout (index : size : status : pid):\n");
    for (int i = 0; i < block_count; ++i) {
        printf("[%d] : %4d : %s : %s\n",
               i,
               mem[i].size,
               mem[i].is_free ? "FREE" : "ALLOC",
               mem[i].is_free ? "-" : mem[i].pid);
    }
    printf("\n");
}

// Initialize blocks (interactive)
void init_blocks_interactive() {
    int n;
    printf("Enter number of initial memory blocks: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_BLOCKS) {
        printf("Invalid number. Using 5 blocks default.\n");
        n = 5;
    }
    block_count = n;
    for (int i = 0; i < block_count; ++i) {
        int s;
        printf("Enter size of block %d: ", i);
        if (scanf("%d", &s) != 1 || s <= 0) {
            printf("Invalid size. Using 100.\n");
            s = 100;
        }
        mem[i].size = s;
        mem[i].is_free = 1;
        mem[i].pid[0] = '\0';
    }
    print_memory();
}

void init_blocks_example() {
    // Example initial blocks for quick testing
    int sizes[] = {100, 500, 200, 300, 600};
    block_count = 5;
    for (int i = 0; i < block_count; ++i) {
        mem[i].size = sizes[i];
        mem[i].is_free = 1;
        mem[i].pid[0] = '\0';
    }
    printf("Initialized example memory blocks.\n");
    print_memory();
}

// Helper: split a block into allocated part and a new free remainder (if remainder > 0)
void split_block(int index, int alloc_size, const char *pid) {
    if (alloc_size <= 0 || index < 0 || index >= block_count) return;
    int rem = mem[index].size - alloc_size;
    strcpy(mem[index].pid, pid);
    mem[index].is_free = 0;
    mem[index].size = alloc_size;
    if (rem > 0) {
        // shift blocks right to insert remainder block
        if (block_count + 1 > MAX_BLOCKS) {
            // No space to split (rare in simulation) — treat as exact fit
            return;
        }
        for (int i = block_count; i > index + 1; --i) mem[i] = mem[i - 1];
        mem[index + 1].size = rem;
        mem[index + 1].is_free = 1;
        mem[index + 1].pid[0] = '\0';
        block_count++;
    }
}

// Coalesce adjacent free blocks
void coalesce_free() {
    int i = 0;
    while (i < block_count - 1) {
        if (mem[i].is_free && mem[i + 1].is_free) {
            mem[i].size += mem[i + 1].size;
            // shift left
            for (int j = i + 1; j < block_count - 1; ++j) mem[j] = mem[j + 1];
            block_count--;
            // do not increment i, check again at same index
        } else {
            i++;
        }
    }
}

// Allocation strategies
int first_fit(const char *pid, int req_size) {
    for (int i = 0; i < block_count; ++i) {
        if (mem[i].is_free && mem[i].size >= req_size) {
            split_block(i, req_size, pid);
            return 1; // success
        }
    }
    return 0; // fail
}

int best_fit(const char *pid, int req_size) {
    int best_i = -1;
    int best_size = 1<<30;
    for (int i = 0; i < block_count; ++i) {
        if (mem[i].is_free && mem[i].size >= req_size) {
            if (mem[i].size < best_size) {
                best_size = mem[i].size;
                best_i = i;
            }
        }
    }
    if (best_i != -1) {
        split_block(best_i, req_size, pid);
        return 1;
    }
    return 0;
}

int worst_fit(const char *pid, int req_size) {
    int worst_i = -1;
    int worst_size = -1;
    for (int i = 0; i < block_count; ++i) {
        if (mem[i].is_free && mem[i].size >= req_size) {
            if (mem[i].size > worst_size) {
                worst_size = mem[i].size;
                worst_i = i;
            }
        }
    }
    if (worst_i != -1) {
        split_block(worst_i, req_size, pid);
        return 1;
    }
    return 0;
}

// Deallocate by pid
int deallocate_pid(const char *pid) {
    int found = 0;
    for (int i = 0; i < block_count; ++i) {
        if (!mem[i].is_free && strcmp(mem[i].pid, pid) == 0) {
            mem[i].is_free = 1;
            mem[i].pid[0] = '\0';
            found = 1;
        }
    }
    if (found) coalesce_free();
    return found;
}

// Menu-driven interaction
void interactive_menu() {
    while (1) {
        printf("=== Memory Allocation Simulator ===\n");
        printf("1. Initialize blocks (interactive)\n");
        printf("2. Initialize example blocks\n");
        printf("3. Allocate (First Fit)\n");
        printf("4. Allocate (Best Fit)\n");
        printf("5. Allocate (Worst Fit)\n");
        printf("6. Deallocate by PID\n");
        printf("7. Show memory map\n");
        printf("8. Exit\n");
        printf("Enter choice: ");

        int choice;
        if (scanf("%d", &choice) != 1) { // handle EOF or invalid
            printf("Invalid input. Exiting.\n");
            return;
        }

        if (choice == 1) {
            init_blocks_interactive();
        } else if (choice == 2) {
            init_blocks_example();
        } else if (choice == 3 || choice == 4 || choice == 5) {
            char pid[16];
            int size;
            printf("Enter process id (string): ");
            scanf("%s", pid);
            printf("Enter required size: ");
            if (scanf("%d", &size) != 1 || size <= 0) {
                printf("Invalid size.\n");
                continue;
            }
            int ok = 0;
            if (choice == 3) ok = first_fit(pid, size);
            else if (choice == 4) ok = best_fit(pid, size);
            else ok = worst_fit(pid, size);

            if (ok) {
                printf("Allocation successful.\n");
            } else {
                printf("Allocation failed — no suitable block.\n");
            }
            print_memory();
        } else if (choice == 6) {
            char pid[16];
            printf("Enter PID to deallocate: ");
            scanf("%s", pid);
            if (deallocate_pid(pid)) {
                printf("Deallocated blocks of PID %s.\n", pid);
            } else {
                printf("No block found for PID %s.\n", pid);
            }
            print_memory();
        } else if (choice == 7) {
            print_memory();
        } else if (choice == 8) {
            printf("Exiting...\n");
            return;
        } else {
            printf("Invalid choice.\n");
        }
    }
}

int main() {
    // start interactive session
    printf("Memory Allocation Strategies Simulator\n");
    printf("You can initialize blocks or use example blocks to try allocations.\n\n");
    init_blocks_example(); // default example
    interactive_menu();
    return 0;
}

