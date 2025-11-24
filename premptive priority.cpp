#include <stdio.h>
#include <limits.h>

int main() {
    int n;
    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) return 0;

    int at[50], bt[50], pr[50], rt[50], ct[50], tat[50], wt[50];
    for (int i = 0; i < n; ++i) {
        printf("AT BT PRI for P%d: ", i+1);
        scanf("%d %d %d", &at[i], &bt[i], &pr[i]);
        rt[i] = bt[i];
        ct[i] = tat[i] = wt[i] = 0;
    }

    int completed = 0, t = 0;
    while (completed < n) {
        int idx = -1, best = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (at[i] <= t && rt[i] > 0) {
                if (pr[i] < best ||
                    (pr[i] == best && at[i] < at[idx]) ||
                    (pr[i] == best && at[i] == at[idx] && i < idx)) {
                    best = pr[i];
                    idx = i;
                }
            }
        }

        if (idx == -1) { 
            t++;
            continue;
        }

       
        rt[idx]--;
        t++;

    
        if (rt[idx] == 0) {
            completed++;
            ct[idx] = t;
            tat[idx] = ct[idx] - at[idx];
            wt[idx] = tat[idx] - bt[idx];
        }
    }

    double totWT = 0.0, totTAT = 0.0;
    printf("\nPID\tAT\tBT\tPRI\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; ++i) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               i+1, at[i], bt[i], pr[i], ct[i], tat[i], wt[i]);
        totWT += wt[i]; totTAT += tat[i];
    }

    printf("\nAverage Waiting Time    = %.2f\n", totWT / n);
    printf("Average Turnaround Time = %.2f\n", totTAT / n);

    return 0;
}

