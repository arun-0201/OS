/*
gcc -o banker deadlock.c
./banker

*/
#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int numProcesses, numResources;
int available[MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

void calculateNeed() {
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

bool isSafe(int work[], bool finish[]) {
    int safeSequence[MAX_PROCESSES];
    int count = 0;

    for (int i = 0; i < numProcesses; i++) {
        finish[i] = false;
    }

    while (count < numProcesses) {
        bool found = false;
        for (int p = 0; p < numProcesses; p++) {
            if (!finish[p]) {
                bool canProceed = true;
                for (int r = 0; r < numResources; r++) {
                    if (need[p][r] > work[r]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    for (int r = 0; r < numResources; r++) {
                        work[r] += allocation[p][r];
                    }
                    safeSequence[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) {
            printf("System is not in a safe state.\n");
            return false;
        }
    }

    printf("System is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < numProcesses; i++) {
        printf("P%d ", safeSequence[i]);
    }
    printf("\n");
    return true;
}

bool requestResources(int processID, int request[]) {
    for (int r = 0; r < numResources; r++) {
        if (request[r] > need[processID][r]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
    }

    for (int r = 0; r < numResources; r++) {
        if (request[r] > available[r]) {
            printf("Resources are not available. Process must wait.\n");
            return false;
        }
    }

    // Pretend to allocate requested resources
    for (int r = 0; r < numResources; r++) {
        available[r] -= request[r];
        allocation[processID][r] += request[r];
        need[processID][r] -= request[r];
    }

    int work[MAX_RESOURCES];
    for (int r = 0; r < numResources; r++) {
        work[r] = available[r];
    }

    bool finish[MAX_PROCESSES];
    if (isSafe(work, finish)) {
        printf("Request granted.\n");
        return true;
    } else {
        // Rollback
        for (int r = 0; r < numResources; r++) {
            available[r] += request[r];
            allocation[processID][r] -= request[r];
            need[processID][r] += request[r];
        }
        printf("Request cannot be granted (would lead to unsafe state).\n");
        return false;
    }
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &numProcesses);
    printf("Enter number of resources: ");
    scanf("%d", &numResources);

    printf("Enter available resources: ");
    for (int i = 0; i < numResources; i++) {
        scanf("%d", &available[i]);
    }

    printf("Enter maximum resources for each process:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter allocation for each process:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("Process %d: ", i);
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    calculateNeed();

    int work[MAX_RESOURCES];
    for (int i = 0; i < numResources; i++) {
        work[i] = available[i];
    }

    bool finish[MAX_PROCESSES];
    isSafe(work, finish);

    int request[MAX_RESOURCES];
    printf("Enter request for process (format: P0 1 0 2): ");
    int processID;
    scanf("%d", &processID);
    for (int i = 0; i < numResources; i++) {
        scanf("%d", &request[i]);
    }

    requestResources(processID, request);
    return 0;
}

/*
Enter number of processes: 5
Enter number of resources: 3
Enter available resources: 3
3
2
Enter maximum resources for each process:
Process 0: 7
5
3
Process 1: 3
2
2
Process 2: 9
0
2
Process 3: 2
2
2
Process 4: 4
3
3
Enter allocation for each process:
Process 0: 0
1
0
Process 1: 1
0
0
Process 2: 3
0
2
Process 3: 2
1
1
Process 4: 0
0
2
System is in a safe state.
Safe sequence is: P1 P3 P4 P2 P0 
Enter request for process (format: P0 1 0 2): p1 1 0 2
System is in a safe state.
Safe sequence is: P1 P3 P4 P2 P0 
Request granted.



*/
