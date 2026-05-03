// Shared digital library accessed by reader (students) and writer (faculty) threads
// Multiple readers allowed concurrently
// Writers require exclusive access (no readers or writers during update)
// Must ensure mutual exclusion, data consistency, and race condition prevention
// Must avoid writer starvation under heavy reader load

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define numReaders 5
#define numWriters 2 

// shared state
int readerCount = 0;
int database = 0;

pthread_mutex_t mutex;          // protects readerCount
pthread_mutex_t queueMutex;     // prevents writer starvation
sem_t rwSemaphore;              // controls exclusive database access 

int totalReads = 0;
int totalWrites = 0;

void *reader(void *arg) {
    int id = *(int *)arg;

    // entry
    pthread_mutex_lock(&queueMutex);
    pthread_mutex_lock(&mutex);
    
    readerCount++;
    if (readerCount == 1) {
        sem_wait(&rwSemaphore);
    }
    
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&queueMutex);     
    
    // critical section
    printf("Reader %d reading db = %d\n", id, database);
    
    // exit
    pthread_mutex_lock(&mutex);
    totalReads++;                   
    readerCount--;
    if (readerCount == 0) {
        sem_post(&rwSemaphore);
    }
    
    pthread_mutex_unlock(&mutex);

    printf("Reader %d finished.\n", id);
    pthread_exit(0);
}

void *writer(void *arg) {
    int id = *(int *)arg;

    // entry
    printf("Writer %d waiting for exclusive access...\n", id);
    pthread_mutex_lock(&queueMutex);
    sem_wait(&rwSemaphore);
    
    // critical section
    database += (id * 10);
    printf("Writer %d wrote db = %d\n", id, database);
    totalWrites++;
    
    // exit
    sem_post(&rwSemaphore);
    pthread_mutex_unlock(&queueMutex);
    
    printf("Writer %d finished.\n", id);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    printf("\nUniversity Digital Library (Reader-Writer Simulation)\n");
    printf("Readers: %d   Writers: %d   Database start: 0\n\n", numReaders, numWriters);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&queueMutex, NULL);
    sem_init(&rwSemaphore, 0, 1);

    pthread_t readers[numReaders], writers[numWriters];
    int readerIds[numReaders], writerIds[numWriters];

    for (int i = 0; i < numWriters; i++) {
        writerIds[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writerIds[i]);
    }
    for (int i = 0; i < numReaders; i++) {
        readerIds[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &readerIds[i]);
    }

    for (int i = 0; i < numWriters; i++) {
        pthread_join(writers[i], NULL);
    }        

    for (int i = 0; i < numReaders; i++) {
        pthread_join(readers[i], NULL);
    }

    printf("\nFinal Report (for analysis)\n");
    printf("Final DB value: %d\n", database);
    printf("Total reads: %d\n", totalReads);
    printf("Total writes: %d\n", totalWrites);
    printf("\n");

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&queueMutex);
    sem_destroy(&rwSemaphore);

    return 0;
}