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
#include <unistd.h>

#define bufferSize  5
#define numProducers 3
#define numConsumers 2
#define ordersPerProducer 5   

// shared state
int buffer[bufferSize];
int head = 0, tail = 0;

pthread_mutex_t mutex;      // protects buffer, head, tail
sem_t empty_slots;          // counts free slots
sem_t filled_slots;         // counts filled slots

void *producer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < ordersPerProducer; i++) {
        // simulate varying order arrival rates
        usleep((100 + rand() % 300) * 1000);
        int order = rand() % 100 + 1;

        // entry
        sem_wait(&empty_slots);
        pthread_mutex_lock(&mutex);

        // critical section
        buffer[tail] = order;
        tail = (tail + 1) % bufferSize;
        printf("Producer %d added order %d\n", id, order);

        // exit
        pthread_mutex_unlock(&mutex);
        sem_post(&filled_slots);
    }

    pthread_exit(0);
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < (numProducers * ordersPerProducer) / numConsumers; i++) {
        // entry
        sem_wait(&filled_slots);
        pthread_mutex_lock(&mutex);

        // critical section
        int order = buffer[head];
        head = (head + 1) % bufferSize;
        printf("Consumer %d picked order %d\n", id, order);

        // exit
        pthread_mutex_unlock(&mutex);
        sem_post(&empty_slots);

        // simulate delivery delay (slower)
        usleep((1000 + rand() % 500) * 1000);
    }

    pthread_exit(0);
}

int main(void) {
    srand(time(NULL));
    printf("\nFood Delivery Company (Producer-Consumer Simulation)\n");

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty_slots, 0, bufferSize);
    sem_init(&filled_slots, 0, 0);

    pthread_t producers[numProducers], consumers[numConsumers];
    int producerIds[numProducers], consumerIds[numConsumers];

    for (int i = 0; i < numProducers; i++) {
        producerIds[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producerIds[i]);
    }

    for (int i = 0; i < numConsumers; i++) {
        consumerIds[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &consumerIds[i]);
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

    printf("\nSimulation complete.\n");

    return 0;
}