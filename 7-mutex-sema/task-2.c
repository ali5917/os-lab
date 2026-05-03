// Real-time order system with bounded buffer (fixed-size queue)
// Producers (customers) add orders; consumers (delivery agents) process orders
// Must block producers when buffer is full and consumers when buffer is empty
// Ensure mutual exclusion, prevent race conditions, and maintain data consistency
// Use mutex for critical section and counting semaphores for empty/full slots
// Support concurrent producer–consumer threads with varying arrival and processing rates
// Handle spikes in orders and processing delays
// Prevent deadlock, buffer overflow/underflow, and unnecessary blocking

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define bufferSize  5
#define numProducers 3
#define numConsumers 2

// shared state
int buffer[bufferSize];
int head = 0, tail = 0;

pthread_mutex_t mutex;      // protects buffer, head, tail
sem_t empty_slots;          // counts free slots   (init = bufferSize)
sem_t filled_slots;         // counts filled slots (init = 0)

void *producer(void *arg) {
    int id = *(int *)arg;
    int order = rand() % 100 + 1;

    // entry
    sem_wait(&empty_slots);         // block if buffer is full
    pthread_mutex_lock(&mutex);

    // critical section
    buffer[tail] = order;
    tail = (tail + 1) % bufferSize;
    printf("Producer %d added order %d\n", id, order);

    // exit
    pthread_mutex_unlock(&mutex);
    sem_post(&filled_slots);        // signal that a new order is available

    pthread_exit(0);
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    // entry
    sem_wait(&filled_slots);        // block if buffer is empty
    pthread_mutex_lock(&mutex);

    // critical section
    int order = buffer[head];
    head = (head + 1) % bufferSize;
    printf("Consumer %d picked order %d\n", id, order);

    // exit
    pthread_mutex_unlock(&mutex);
    sem_post(&empty_slots);         // signal that a slot has been freed

    pthread_exit(0);
}

int main(void) {
    srand(time(NULL));
    printf("\nFood Delivery Company\n");

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_slots, 0, bufferSize);       // all slots free at start
    sem_init(&filled_slots, 0, 0);               // no orders at start

    pthread_t producers[numProducers], consumers[numConsumers];
    int producerIds[numProducers], customerIds[numConsumers];

    for (int i = 0; i < numProducers; i++) {
        producerIds[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producerIds[i]);
    }
    
    for (int i = 0; i < numConsumers; i++) {
        customerIds[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &customerIds[i]);
    }

    for (int i = 0; i < numProducers; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < numConsumers; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);

    return 0;
}