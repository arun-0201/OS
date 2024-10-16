//------------------------------------------Threads-------------------------------------------------------
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
/*gcc -pthread -o threads threads.c
./threads
*/

//------------------------------------------Peterson-------------------------------------------------------
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
volatile bool flag[2] = {false, false};
volatile int turn = 0;
void critical_section(int process_id) {
    printf("Process %d is in the critical section.\n", process_id);
}
void peterson(int process_id) {
    int other = 1 - process_id;
    flag[process_id] = true;
    turn = other;
    while (flag[other] && turn == other) {
    }
    critical_section(process_id);
    flag[process_id] = false;
}
void* process(void* arg) {
    int process_id = *(int*)arg;
    printf("Process %d is doing non-critical work.\n", process_id);
    peterson(process_id);
    printf("Process %d is doing more non-critical work.\n", process_id);
    return NULL;
}

int main() {
    pthread_t threads[2];
    int process_ids[2] = {0, 1};
    for (int i = 0; i < 2; ++i) {
        pthread_create(&threads[i], NULL, process, (void*)&process_ids[i]);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_join(threads[i], NULL);
    }
   
    return 0;
}/*gcc -pthread -o Peterson Peterson.c
./Peterson
*/
//--------------------------------------Producer-Consumer--------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define PRODUCE_COUNT 10
#define CONSUME_COUNT 10
int buffer[BUFFER_SIZE];
int in = 0, out = 0;
sem_t empty, full, mutex;
void* producer(void* param) {
    for (int i = 0; i < PRODUCE_COUNT; ++i) {
        int item = rand() % 100;
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produced: %d\n", item);
        sem_post(&mutex);
        sem_post(&full);
    }
    pthread_exit(NULL);
}

void* consumer(void* param) {
    for (int i = 0; i < CONSUME_COUNT; ++i) {
        sem_wait(&full);
        sem_wait(&mutex);
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumed: %d\n", item);
        sem_post(&mutex);
        sem_post(&empty);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    sem_init(&empty, 0, BUFFER_SIZE); 
    sem_init(&full, 0, 0);            
    sem_init(&mutex, 0, 1);          
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }
    for (int i = 0; i < NUM_PRODUCERS; ++i) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        pthread_join(consumers[i], NULL);
    }
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
/*gcc -pthread -o pc pc.c
./pc
*/
//----------------------------------------Reader-Writer----------------------------------------------------
#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
sem_t x,y;
pthread_t tid;
pthread_t writerthreads[100],readerthreads[100];
int readercount = 0;

void *reader(void* param)
{
    sem_wait(&x);
    readercount++;
    if(readercount==1)
        sem_wait(&y);
    sem_post(&x);
    printf("%d reader is inside\n",readercount);
    usleep(3);
    sem_wait(&x);
    readercount--;
    if(readercount==0)
    {
        sem_post(&y);
    }
    sem_post(&x);
    printf("%d Reader is leaving\n",readercount+1);
    return NULL;
}

void *writer(void* param)
{
    printf("Writer is trying to enter\n");
    sem_wait(&y);
    printf("Writer has entered\n");
    sem_post(&y);
    printf("Writer is leaving\n");
    return NULL;
}

int main()
{
    int n2,i;
    printf("Enter the number of readers:");
    scanf("%d",&n2);
    printf("\n");
    int n1[n2];
    sem_init(&x,0,1);
    sem_init(&y,0,1);
    for(i=0;i<n2;i++)
    {
        pthread_create(&writerthreads[i],NULL,reader,NULL);
        pthread_create(&readerthreads[i],NULL,writer,NULL);
    }
    for(i=0;i<n2;i++)
    {
        pthread_join(writerthreads[i],NULL);
        pthread_join(readerthreads[i],NULL);
    }

}/*
gcc -pthread -o rw rw.c
./rw
*/
//------------------------------------------Bankers--------------------------------------------------------
#include<stdio.h>  
int main()  
{  
    // P0 , P1 , P2 , P3 , P4 are the Process names here  
    int n , m , i , j , k;  
    n = 5; // Number of processes  
    m = 3; // Number of resources  
    int alloc[ 5 ] [ 3 ] = { { 0 , 1 , 0 }, // P0 // Allocation Matrix  
                        { 2 , 0 , 0 } , // P1  
                        { 3 , 0 , 2 } , // P2  
                        { 2 , 1 , 1 } , // P3  
                        { 0 , 0 , 2 } } ; // P4  
    int max[ 5 ] [ 3 ] = { { 7 , 5 , 3 } , // P0 // MAX Matrix  
                    { 3 , 2 , 2 } , // P1  
                    { 9 , 0 , 2 } , // P2  
                    { 2 , 2 , 2 } , // P3  
                    { 4 , 3 , 3 } } ; // P4  
    int avail[3] = { 3 , 3 , 2 } ; // Available Resources  
    int f[n] , ans[n] , ind = 0 ;  
    for (k = 0; k < n; k++) {  
        f[k] = 0;  
    }  
    int need[n][m];  
    for (i = 0; i < n; i++) {  
        for (j = 0; j < m; j++)  
            need[i][j] = max[i][j] - alloc[i][j] ;  
    }  
    int y = 0;  
    for (k = 0; k < 5; k++){  
        for (i = 0; i < n; i++){  
            if (f[i] == 0){  
                int flag = 0;  
                for (j = 0; j < m; j++) {  
                    if(need[i][j] > avail[j]){  
                        flag = 1;  
                        break;  
                    }  
                }  
                if ( flag == 0 ) {  
                    ans[ind++] = i;  
                    for (y = 0; y < m; y++)  
                        avail[y] += alloc[i][y] ;  
                    f[i] = 1;  
                }  
            }  
        }  
    }  
    int flag = 1;   
    for(int i=0;i<n;i++)  
    {  
    if(f[i] == 0)  
    {  
        flag = 0;  
        printf(" The following system is not safe ");  
        break;  
    }  
    }  
    if (flag == 1)  
    {  
    printf(" Following is the SAFE Sequence \ n ");  
    for (i = 0; i < n - 1; i++)  
        printf(" P%d -> " , ans[i]);  
    printf(" P%d ", ans[n - 1]);  
    }  
    return(0);  
} /*gcc -pthread -o bankers bankers.c
./bankers
*/
//-------------------------------------Deadlock-Detection--------------------------------------------------
#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

int allocation[MAX_PROCESSES][MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int available[MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];

// Function to calculate the need matrix
void calculateNeed(int processes, int resources) {
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

// Function to check if the system is in a safe state and find the safe sequence
bool isSafe(int processes, int resources) {
    bool finish[MAX_PROCESSES] = { false };
    int work[MAX_RESOURCES];
    int safeSeq[MAX_PROCESSES];
    
    // Initialize work with available resources
    for (int i = 0; i < resources; i++) {
        work[i] = available[i];
    }

    int count = 0; // Number of processes that can finish

    while (count < processes) {
        bool found = false;

        for (int p = 0; p < processes; p++) {
            if (!finish[p]) {
                bool canAllocate = true;

                // Check if resources are sufficient
                for (int j = 0; j < resources; j++) {
                    if (need[p][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                // If resources are sufficient, simulate allocation
                if (canAllocate) {
                    for (int j = 0; j < resources; j++) {
                        work[j] += allocation[p][j];
                    }
                    finish[p] = true; // Mark process as finished
                    safeSeq[count++] = p; // Record the safe sequence
                    found = true; // Found a process that can finish
                }
            }
        }

        // If no process was found that can finish, we are in a deadlock
        if (!found) {
            return false; // Deadlock detected
        }
    }

    // Print safe sequence
    printf("System is in a safe state.\nSafe sequence is: ");
    for (int i = 0; i < processes; i++) {
        printf("P%d ", safeSeq[i]);
    }
    printf("\n");

    return true; // No deadlock, safe state
}

int main() {
    int numProcesses, numResources;

    // User input for number of processes and resources
    printf("Enter number of processes: ");
    scanf("%d", &numProcesses);
    printf("Enter number of resources: ");
    scanf("%d", &numResources);

    // User input for allocation matrix
    printf("Enter allocation matrix:\n");
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // User input for maximum demand matrix
    printf("Enter max demand matrix:\n");
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // User input for available resources
    printf("Enter available resources:\n");
    for (int i = 0; i < numResources; i++) {
        scanf("%d", &available[i]);
    }

    calculateNeed(numProcesses, numResources);

    // Check for deadlock and print safe sequence if no deadlock is detected
    if (isSafe(numProcesses, numResources)) {
        printf("No deadlock detected.\n");
    } else {
        printf("Deadlock detected!\n");
    }

    return 0;
}/*gcc -pthread -o DD DD.c
./DD*/
//-------------------------------------Dining-Philosopher--------------------------------------------------
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

sem_t mutex;
sem_t S[N];

void test(int phnum)
{
    if (state[phnum] == HUNGRY
        && state[LEFT] != EATING
        && state[RIGHT] != EATING) {
        // state that eating
        state[phnum] = EATING;

        sleep(2);

        printf("Philosopher %d takes fork %d and %d\n",
                      phnum + 1, LEFT + 1, phnum + 1);

        printf("Philosopher %d is Eating\n", phnum + 1);

        // sem_post(&S[phnum]) has no effect
        // during takefork
        // used to wake up hungry philosophers
        // during putfork
        sem_post(&S[phnum]);
    }
}

// take up chopsticks
void take_fork(int phnum)
{

    sem_wait(&mutex);

    // state that hungry
    state[phnum] = HUNGRY;

    printf("Philosopher %d is Hungry\n", phnum + 1);

    // eat if neighbours are not eating
    test(phnum);

    sem_post(&mutex);

    // if unable to eat wait to be signalled
    sem_wait(&S[phnum]);

    sleep(1);
}

// put down chopsticks
void put_fork(int phnum)
{

    sem_wait(&mutex);

    // state that thinking
    state[phnum] = THINKING;

    printf("Philosopher %d putting fork %d and %d down\n",
           phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is thinking\n", phnum + 1);

    test(LEFT);
    test(RIGHT);

    sem_post(&mutex);
}

void* philosopher(void* num)
{

    while (1) {

        int* i = num;

        sleep(1);

        take_fork(*i);

        sleep(0);

        put_fork(*i);
    }
}

int main()
{

    int i;
    pthread_t thread_id[N];

    // initialize the semaphores
    sem_init(&mutex, 0, 1);

    for (i = 0; i < N; i++)

        sem_init(&S[i], 0, 0);

    for (i = 0; i < N; i++) {

        // create philosopher processes
        pthread_create(&thread_id[i], NULL,
                       philosopher, &phil[i]);

        printf("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)

        pthread_join(thread_id[i], NULL);
}/*gcc -pthread -o DD DD.c
./DD*/
//------------------------------Dining-Philosopher-Without-Deadlock----------------------------------------
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
/*gcc -pthread -o DD DD.c
./DD*/
