// One thread continuously prints "Thread running..."
// Another thread waits for a signal (SIGUSR1)
// When the signal is received, the program should print & terminate 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;

void* workerThread(void* arg) {
    while (running) {
        printf("Thread running...\n");
        sleep(1);
    }
    printf("Signal received in thread. Worker thread exiting!\n");
    pthread_exit(0);  
}

// Signal handler
void handleSigusr1(int sig) {
    running = 0;
}

int main() {
    pthread_t t1;

    struct sigaction sa;
    sa.sa_handler = handleSigusr1;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGUSR1, &sa, NULL);

    pthread_create(&t1, NULL, workerThread, NULL);

    while (running) {
        pause();
    }

    pthread_join(t1, NULL);

    printf("Program terminated gracefully.\n");
    return 0;
}