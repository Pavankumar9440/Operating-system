#include <stdio.h>

int main() {
    int n, i, j;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int bt[n], pr[n], wt[n], tat[n], pid[n];

    for (i = 0; i < n; i++) {
        pid[i] = i + 1;
        printf("Burst time & Priority of P%d: ", pid[i]);
        scanf("%d %d", &bt[i], &pr[i]);
    }

   
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (pr[j] > pr[i]) {
                int t = pr[i]; pr[i] = pr[j]; pr[j] = t;
                t = bt[i]; bt[i] = bt[j]; bt[j] = t;
                t = pid[i]; pid[i] = pid[j]; pid[j] = t;
            }

    wt[0] = 0;
    for (i = 1; i < n; i++)
        wt[i] = wt[i - 1] + bt[i - 1];

    for (i = 0; i < n; i++)
        tat[i] = wt[i] + bt[i];

    printf("\nPID\tPRI\tBT\tWT\tTAT\n");
    for (i = 0; i < n; i++)
        printf("P%d\t%d\t%d\t%d\t%d\n", pid[i], pr[i], bt[i], wt[i], tat[i]);

    return 0;
}

