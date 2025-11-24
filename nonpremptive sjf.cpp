#include <stdio.h>
#include <limits.h>

int main() {
    int n; 
    printf("Enter number of processes: ");
    scanf("%d",&n);

    int at[50], bt[50], ct[50], tat[50], wt[50], done[50]={0};

    for(int i=0;i<n;i++){
        printf("AT BT for P%d: ",i+1);
        scanf("%d %d",&at[i],&bt[i]);
    }

    int time=0, comp=0;
    while(comp<n){
        int idx=-1, minBT=INT_MAX;

        for(int i=0;i<n;i++)
            if(!done[i] && at[i]<=time && bt[i]<minBT){
                minBT = bt[i];
                idx = i;
            }

        if(idx==-1){ time++; continue; }

        time += bt[idx];
        ct[idx]  = time;
        tat[idx] = ct[idx] - at[idx];
        wt[idx]  = tat[idx] - bt[idx];
        done[idx] = 1;
        comp++;
    }

    printf("\nPID AT BT CT TAT WT\n");
    for(int i=0;i<n;i++)
        printf("P%d  %d  %d  %d  %d  %d\n",i+1,at[i],bt[i],ct[i],tat[i],wt[i]);

    return 0;
}

