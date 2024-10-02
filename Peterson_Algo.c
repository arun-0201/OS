/*gcc -o peterson filename.c -pthread
./peterson
*/
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

#define N 2

// Shared variables
int turn;
bool flag[N];

// Critical section function
void critical_section(int id) {
    printf("Process %d in critical section.\n", id);
    sleep(1); // Simulate work
    printf("Process %d leaving critical section.\n", id);
}

void enter_critical_section(int id) {
    int other = 1 - id;
    flag[id] = true;
    turn = other;
    while (flag[other] && turn == other) {}
}

void leave_critical_section(int id) {
    flag[id] = false;
}

void* process(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < 5; ++i) {
        printf("Process %d doing non-critical work.\n", id);
        sleep(1);

        enter_critical_section(id);
        critical_section(id);
        leave_critical_section(id);

        printf("Process %d doing more non-critical work.\n", id);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t threads[N];
    int ids[N] = {0, 1};

    turn = 0;
    flag[0] = flag[1] = false;

    for (int i = 0; i < N; ++i) {
        pthread_create(&threads[i], NULL, process, &ids[i]);
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

/*Output:
Process 0 doing non-critical work.
Process 1 doing non-critical work.
Process 0 in critical section.
Process 0 leaving critical section.
Process 0 doing more non-critical work.
Process 1 in critical section.
Process 0 doing non-critical work.
Process 1 leaving critical section.
Process 1 doing more non-critical work.
Process 0 in critical section.
Process 1 doing non-critical work.
Process 0 leaving critical section.
Process 0 doing more non-critical work.
Process 1 in critical section.
Process 1 leaving critical section.
Process 1 doing more non-critical work.
Process 0 doing non-critical work.
Process 1 doing non-critical work.
Process 0 in critical section.
Process 0 leaving critical section.
Process 0 doing more non-critical work.
Process 1 in critical section.
Process 0 doing non-critical work.
Process 1 leaving critical section.
Process 1 doing more non-critical work.
Process 0 in critical section.
Process 1 doing non-critical work.
Process 0 leaving critical section.
Process 0 doing more non-critical work.
Process 1 in critical section.
Process 0 doing non-critical work.
Process 1 leaving critical section.
Process 1 doing more non-critical work.
Process 0 in critical section.
Process 1 doing non-critical work.
Process 0 leaving critical section.
Process 0 doing more non-critical work.
Process 1 in critical section.
Process 1 leaving critical section.
Process 1 doing more non-critical work.
*/
