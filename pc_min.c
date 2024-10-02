/*gcc -pthread filename.c
./a.out
*/
#include <stdio.h> 
#include <semaphore.h> 
#include <pthread.h> 
#include <unistd.h> 

#define BUFFER_SIZE 5

// Shared buffer and indices
int front = 0; 
int rear = -1; 
int buffer[BUFFER_SIZE];

// Semaphores for synchronization
sem_t mutex; 
sem_t emptyCount; 
sem_t fullCount; 

// Function to generate a new item
int produce_item() 
{ 
    static int item = 100; 
    return item++; 
} 

// Function to insert an item into the buffer
void insert_item(int item) 
{ 
    rear = (rear + 1) % BUFFER_SIZE; 
    buffer[rear] = item; 
} 

// Function to remove an item from the buffer
int remove_item() 
{ 
    int item = buffer[front]; 
    front = (front + 1) % BUFFER_SIZE; 
    return item; 
} 

// Producer thread function
void *produce(void *arg) 
{ 
    int item; 
    while (1) 
    { 
        item = produce_item(); 
        sem_wait(&emptyCount); // Wait for an empty slot
        sem_wait(&mutex);     // Enter critical section

        printf("\nProducer entering critical section"); 
        insert_item(item); 
        printf("\nProducer inserted item %d at %d", item, rear); 

        sem_post(&mutex);     // Exit critical section
        sem_post(&fullCount); // Signal that a new item is available
    } 
} 

// Consumer thread function
void *consume(void *arg) 
{ 
    int item; 
    while (1) 
    { 
        sleep(5); 
        sem_wait(&fullCount); // Wait for an item to consume
        sem_wait(&mutex);     // Enter critical section

        printf("\nConsumer entering critical section"); 
        item = remove_item(); 
        printf("\nConsumer consumed item %d", item); 

        sem_post(&mutex);     // Exit critical section
        sem_post(&emptyCount); // Signal that a slot is available
    } 
} 

int main() 
{ 
    pthread_t producer_thread; 
    pthread_t consumer_thread; 

    // Initialize semaphores
    sem_init(&mutex, 0, 1); 
    sem_init(&emptyCount, 0, BUFFER_SIZE); 
    sem_init(&fullCount, 0, 0); 

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, produce, NULL); 
    pthread_create(&consumer_thread, NULL, consume, NULL); 

    // Wait for threads to finish (they won't in this infinite loop example)
    pthread_join(producer_thread, NULL); 
    pthread_join(consumer_thread, NULL); 

    return 0; 
}
/*
Producer entering critical section
Producer inserted item 100 at 0
Producer entering critical section
Producer inserted item 101 at 1
Producer entering critical section
Producer inserted item 102 at 2
Producer entering critical section
Producer inserted item 103 at 3
Producer entering critical section
Producer inserted item 104 at 4
Consumer entering critical section
Consumer consumed item 100
Producer entering critical section
Producer inserted item 105 at 0
Consumer entering critical section
Consumer consumed item 101
Producer entering critical section
Producer inserted item 106 at 1
Consumer entering critical section
Consumer consumed item 102
Producer entering critical section
Producer inserted item 107 at 2
Consumer entering critical section
Consumer consumed item 103
Producer entering critical section
Producer inserted item 108 at 3
Consumer entering critical section
Consumer consumed item 104
Producer entering critical section
Producer inserted item 109 at 4
Consumer entering critical section
Consumer consumed item 105
Producer entering critical section
*/
