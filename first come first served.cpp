#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    int *burst = (int*) malloc(n * sizeof(int));
    int *completion = (int*) malloc(n * sizeof(int));
    int *turnaround = (int*) malloc(n * sizeof(int));
    int *waiting = (int*) malloc(n * sizeof(int));

    for (i = 0; i < n; ++i) {
        printf("Enter burst time for P%d: ", i+1);
        if (scanf("%d", &burst[i]) != 1 || burst[i] < 0) {
            printf("Invalid burst time.\n");
            free(burst); free(completion); free(turnaround); free(waiting);
            return 1;
        }
    }

    // since all arrival times = 0 and FCFS: order is input order
    completion[0] = burst[0];
    for (i = 1; i < n; ++i) {
        completion[i] = completion[i-1] + burst[i];
    }

    double totalTAT = 0.0, totalWT = 0.0;
    for (i = 0; i < n; ++i) {
        turnaround[i] = completion[i];        // arrival = 0
        waiting[i] = turnaround[i] - burst[i];
        totalTAT += turnaround[i];
        totalWT += waiting[i];
    }

    printf("\n+------+-----------+-------------+--------------+-----------+\n");
    printf("| PID  | BurstTime | Completion  | Turnaround   | Waiting   |\n");
    printf("+------+-----------+-------------+--------------+-----------+\n");
    for (i = 0; i < n; ++i) {
        printf("|  P%-3d|    %-6d |    %-8d |    %-8d   |   %-6d |\n",
               i+1, burst[i], completion[i], turnaround[i], waiting[i]);
    }
    printf("+------+-----------+-------------+--------------+-----------+\n");

    printf("\nAverage Turnaround Time = %.2f\n", totalTAT / n);
    printf("Average Waiting Time    = %.2f\n", totalWT / n);

    // Simple Gantt chart (shows process order and times)
    printf("\nGANTT CHART:\n|");
    for (i = 0; i < n; ++i) {
        printf(" P%d |", i+1);
    }
    printf("\n0");
    for (i = 0; i < n; ++i) {
        printf("    %d", completion[i]);
    }
    printf("\n");

    free(burst);
    free(completion);
    free(turnaround);
    free(waiting);

    return 0;
}

