// Multi-threaded Signal Handling
// Thread 1: Continuously prints "Thread running..."
// Thread 2: Waits for SIGUSR1 using pause()
// Main thread: Joins both threads after the signal is received

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

// Global flag to control thread execution
volatile sig_atomic_t running = 1;

// Signal handler to update the flag
void handleSignal(int sig) {
    running = 0;
}

// Thread 1 Function: Printing
void* printerThread(void* arg) {
    while (running) {
        printf("Printer Thread: Still running...\n");
        sleep(1);
    }
    printf("Printer Thread: Signal detected! Exiting...\n");
    return NULL;
}

// Thread 2 Function: Waiting
void* waiterThread(void* arg) {
    printf("Waiter Thread: Waiting for SIGUSR1 signal...\n");
    while (running) {
        pause(); // Wait for any signal to arrive
    }
    printf("Waiter Thread: Noticed signal! Exiting...\n");
    return NULL;
}

int main() {
    pthread_t t1, t2;

    struct sigaction sa;
    sa.sa_handler = handleSignal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("Main: Creating threads. Send SIGUSR1 (kill -USR1 <pid>) to terminate.\n\n");

    // Create the printer and waiter threads
    pthread_create(&t1, NULL, printerThread, NULL);
    pthread_create(&t2, NULL, waiterThread, NULL);

    // Wait for both threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nMain: Both threads joined. Program terminated gracefully.\n");
    return 0;
}
