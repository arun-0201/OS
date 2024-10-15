#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Function that each thread will execute
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    printf("Thread %d starting\n", thread_id);
    sleep(2);  // Simulate work by sleeping for 2 seconds
    printf("Thread %d finishing\n", thread_id);
    return NULL;
}

int main() {
    const int num_threads = 5;
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    // Create threads
    for (int i = 0; i < num_threads; ++i) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, (void*)&thread_ids[i]) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; ++i) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed to join thread");
            exit(EXIT_FAILURE);
        }
    }

    printf("All threads have finished execution.\n");

    return 0;
}
