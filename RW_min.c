/*gcc -pthread filename.c
./a.out*/
#include <stdio.h> 
#include <semaphore.h> 
#include <pthread.h> 
#include <unistd.h> 

sem_t mutex;    // Semaphore for mutual exclusion among readers
sem_t rw_mutex; // Semaphore for mutual exclusion among writers
int readcount = 0; // Count of active readers
int ticket = 0;    // Shared resource

void book() { ticket++; }  // Function to book a ticket (increment)
void cancel() { ticket--; } // Function to cancel a ticket (decrement)

// Reader thread function
void *reader(void *arg) {
    while (1) {
        sem_wait(&mutex); // Enter critical section for readers
        readcount++;
        if (readcount == 1)
            sem_wait(&rw_mutex); // First reader locks the writer
        sem_post(&mutex); // Exit critical section for readers

        // Reading section
        printf("Ticket value = %d\n", ticket);

        sem_wait(&mutex); // Enter critical section for readers
        readcount--;
        if (readcount == 0)
            sem_post(&rw_mutex); // Last reader releases the writer
        sem_post(&mutex); // Exit critical section for readers

        sleep(3); // Simulate reading time
    }
    return NULL;
}

// Writer thread function
void *writer(void *arg) {
    char op;
    while (1) {
        sleep(3); // Simulate some delay

        sem_wait(&rw_mutex); // Enter critical section for writers

        // Writing section
        printf("Enter 'b' to book or 'c' to cancel: ");
        scanf(" %c", &op);
        if (op == 'b') book();
        if (op == 'c') cancel();

        sem_post(&rw_mutex); // Exit critical section for writers
    }
    return NULL;
}

int main() {
    pthread_t r_tid, w_tid;
    sem_init(&mutex, 0, 1); // Initialize mutex semaphore
    sem_init(&rw_mutex, 0, 1); // Initialize rw_mutex semaphore

    pthread_create(&r_tid, NULL, reader, NULL); // Create reader thread
    pthread_create(&w_tid, NULL, writer, NULL); // Create writer thread

    pthread_join(r_tid, NULL); // Wait for reader thread to finish
    pthread_join(w_tid, NULL); // Wait for writer thread to finish

    return 0;
}
/*
Ticket value = 0
Ticket value = 0
Enter 'b' to book or 'c' to cancel: b
Ticket value = 1
Ticket value = 1
Enter 'b' to book or 'c' to cancel: b
Ticket value = 2
Enter 'b' to book or 'c' to cancel: b
Ticket value = 3
Enter 'b' to book or 'c' to cancel: b
Ticket value = 4
Enter 'b' to book or 'c' to cancel: c
Ticket value = 3
Ticket value = 3
Enter 'b' to book or 'c' to cancel: b
Ticket value = 4
Enter 'b' to book or 'c' to cancel: b
Ticket value = 5
Enter 'b' to book or 'c' to cancel: b
Ticket value = 6
Enter 'b' to book or 'c' to cancel: c
Ticket value = 5
Enter 'b' to book or 'c' to cancel: c
Ticket value = 4
Ticket value = 4
Enter 'b' to book or 'c' to cancel: c
Ticket value = 3
cEnter 'b' to book or 'c' to cancel: c
Ticket value = 2
Enter 'b' to book or 'c' to cancel: Ticket value = 1
c
Enter 'b' to book or 'c' to cancel: Ticket value = 0
Enter 'b' to book or 'c' to cancel: c
Ticket value = -1
Ticket value = -1
Enter 'b' to book or 'c' to cancel: b
Ticket value = 0
Ticket value = 0
Enter 'b' to book or 'c' to cancel: b
Ticket value = 1
b
Enter 'b' to book or 'c' to cancel: b
Ticket value = 2
Enter 'b' to book or 'c' to cancel: ^C

*/
