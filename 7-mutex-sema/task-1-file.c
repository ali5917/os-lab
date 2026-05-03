#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define numReaders 5
#define numWriters 2
#define FILENAME "library.txt"

pthread_mutex_t mutex;
pthread_mutex_t queueMutex;
sem_t rwSemaphore;

int readerCount = 0;
int totalReads  = 0;
int totalWrites = 0;

void *reader(void *arg) {
    int id = *(int *)arg;

    // entry
    pthread_mutex_lock(&queueMutex);
    pthread_mutex_lock(&mutex);
    readerCount++;
    if (readerCount == 1)
        sem_wait(&rwSemaphore);
    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&queueMutex);

    // critical section — read from file
    FILE *fPtr = fopen(FILENAME, "r");
    if (fPtr) {
        char line[256];
        printf("Reader %d reading: ", id);
        while (fgets(line, sizeof(line), fPtr)) {
            printf("%s", line);
        }
        printf("\n");
        fclose(fPtr);
    }

    // exit
    pthread_mutex_lock(&mutex);
    totalReads++;
    readerCount--;
    if (readerCount == 0)
        sem_post(&rwSemaphore);
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

    // critical section — write to file
    FILE *fPtr = fopen(FILENAME, "a");
    if (fPtr) {
        fprintf(f, "Update by faculty member %d\n", id);
        fclose(fPtr);
        printf("Writer %d wrote to file.\n", id);
    }
    totalWrites++;

    // exit
    sem_post(&rwSemaphore);
    pthread_mutex_unlock(&queueMutex);

    printf("Writer %d finished.\n", id);
    pthread_exit(0);
}

int main(void) {
    printf("\nUniversity Digital Library (File-based)\n\n");

    // create the file fresh each run
    FILE *f = fopen(FILENAME, "w");
    fprintf(f, "Initial document content.\n");
    fclose(f);

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

    for (int i = 0; i < numWriters; i++) pthread_join(writers[i], NULL);
    for (int i = 0; i < numReaders; i++) pthread_join(readers[i], NULL);

    printf("\nFinal Report\n");
    printf("Total reads : %d\n", totalReads);
    printf("Total writes: %d\n", totalWrites);

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&queueMutex);
    sem_destroy(&rwSemaphore);

    return 0;
}