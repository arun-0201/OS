#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];  // State of each philosopher
sem_t mutex;   // Mutex for updating states
sem_t s[N];    // Semaphores for each philosopher

// Check if the philosopher can start eating
void test(int i) {
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        sem_post(&s[i]);  // Allow the philosopher to eat
    }
}

// Philosopher tries to pick up forks
void take_forks(int i) {
    sem_wait(&mutex);   // Enter critical section
    state[i] = HUNGRY; // Mark philosopher as hungry
    printf("\nPhilosopher %d is hungry", i);
    test(i);           // Check if the philosopher can eat
    sem_post(&mutex);  // Exit critical section
    sem_wait(&s[i]);   // Wait until the philosopher can eat
}

// Philosopher puts down forks
void put_forks(int i) {
    sem_wait(&mutex);   // Enter critical section
    state[i] = THINKING;  // Mark philosopher as thinking
    test(LEFT);       // Check if the left neighbor can eat
    test(RIGHT);      // Check if the right neighbor can eat
    sem_post(&mutex);  // Exit critical section
}

// Philosopher's life cycle
void *philosopher(void *arg) {
    int i = *(int *)arg;
    while (1) {
        printf("\nPhilosopher %d is thinking", i);
        sleep(5);        // Simulate thinking
        take_forks(i);   // Try to take forks and eat
        printf("\nPhilosopher %d is eating", i);
        sleep(5);        // Simulate eating
        put_forks(i);    // Put down forks
    }
    return NULL;
}

int main() {
    pthread_t threads[N];
    sem_init(&mutex, 0, 1);  // Initialize the mutex semaphore
    int philosophers[N] = {0, 1, 2, 3, 4};
	int i;
    // Initialize semaphores for each philosopher
    for ( i = 0; i < N; i++) {
        sem_init(&s[i], 0, 0);
    }

    // Create philosopher threads
    for (i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, philosopher, &philosophers[i]);
    }

    // Wait for philosopher threads to finish (they won't in this case)
    for ( i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

