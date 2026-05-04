// SIGINT handling, instead of terminating immediately, the program should:
// Print "Interrupt signal received!"
// Ask the user whether they really want to exit (Y/N)
// Exit only if the user enters Y


#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// flag set by handler — volatile so compiler never caches it in a register
volatile sig_atomic_t interrupted = 0;

void sigintHandler(int signum) {
    interrupted = 1;    // signal-safe: just set a flag, nothing else
}

int main() {
    struct sigaction sa;
    sa.sa_handler = sigintHandler;
    sigemptyset(&sa.sa_mask);   // no extra signals blocked during handler    
    sa.sa_flags = 0;            // no SA_RESTART: pause() must return on signal
    sigaction(SIGINT, &sa, NULL);

    printf("Program running. Press Ctrl+C to interrupt.\n");

    while (1) {
        pause();                // sleeps until ANY signal arrives
        if (interrupted) {
            char choice;
            interrupted = 0;    // reset flag for next time
            printf("\nInterrupt signal received!\n");
            printf("Do you really want to exit? (Y/N): ");
            scanf(" %c", &choice);    // space before %c skips leftover newline
            
            if (choice == 'Y' || choice == 'y') {
                printf("Exiting...\n");
                return 0;
            } else {
                printf("Resuming. Press Ctrl+C again to try.\n");
            }
        }
    }

    return 0;
}