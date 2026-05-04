// Practice Task: The Coffee Shop Simulation
// A busy coffee shop needs to safely coordinate its customers and baristas.
// 
// Constraints & Rules:
// 1. The shop has a strict fire-code capacity: only 5 customers can be inside at any given time.
//    If the shop is full, arriving customers must wait outside.
// 2. There are exactly 2 Baristas. A Barista can only serve one customer at a time.
//    If a customer is inside but both baristas are busy, the customer waits in line.
// 3. The shop maintains a shared variable `total_revenue`. After being served, a customer 
//    pays $5, which must be safely added to the revenue.
// 
// Simulation Requirements:
// - Create 15 Customer threads.
// - When a Customer runs, it should: Enter the shop -> Get served (sleep for 1 sec) -> Pay -> Leave.
// - Print status updates (e.g., "Customer 3 entered", "Customer 3 being served", "Customer 3 leaving").
// - The main thread must wait for all customers to finish and then print the final `total_revenue`.
// 
// Your Task:
// - Determine which synchronization tools (mutexes, counting semaphores, etc.) are needed to 
//   enforce the shop capacity, barista availability, and safe revenue tracking.
// - Implement them correctly to prevent race conditions, deadlocks, and starvation.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define numCustomers 15

int totalRevenue = 0;

sem_t freeC;   // counting semaphore to track customers
sem_t freeB;   // counting semaphore to track barista
pthread_mutex_t rev; 

void* handleC(void* arg) {
    int id = *(int *)arg;

    // 1. Enter the shop (wait for capacity)
    sem_wait(&freeC);
    printf("Customer %d entered the shop.\n", id);

    // 2. Wait for a Barista
    sem_wait(&freeB);
    printf("Customer %d is being served by a Barista...\n", id);
    sleep(1); // Simulating being served
    
    // 3. Pay (safely update shared revenue)
    pthread_mutex_lock(&rev);
    totalRevenue += 5;
    pthread_mutex_unlock(&rev);
    
    // 4. Leave
    printf("Customer %d paid and is leaving.\n", id);
    sem_post(&freeB); // Free the Barista
    sem_post(&freeC); // Free a spot in the shop

    pthread_exit(NULL);
}

int main () {
    // Initialize Semaphores and Mutex
    sem_init(&freeC, 0, 5); // Max 5 customers in shop
    sem_init(&freeB, 0, 2); // 2 Baristas
    pthread_mutex_init(&rev, NULL);

    pthread_t threads[numCustomers];
    int ids[numCustomers]; // Array to hold unique IDs to prevent race condition

    for (int i = 0; i < numCustomers; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, handleC, &ids[i]);
    }

    for (int i = 0; i < numCustomers; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nFinal Total Revenue: $%d\n", totalRevenue);

    sem_destroy(&freeC);
    sem_destroy(&freeB);
    pthread_mutex_destroy(&rev);

    return 0;
}