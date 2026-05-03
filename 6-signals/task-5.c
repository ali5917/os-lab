// Registers handlers for SIGUSR1 and SIGUSR2 When:
// SIGUSR1 is received → print "User-defined signal 1 received"
// SIGUSR2 is received → print "User-defined signal 2 received"
// The program should run continuously and respond to both signals

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sigusr1Flag = 0;
volatile sig_atomic_t sigusr2Flag = 0;

void handleSigusr1(int sig) {
    sigusr1Flag = 1;
}

void handleSigusr2(int sig) {
    sigusr2Flag = 1;
}

int main() {
    struct sigaction sa1, sa2;

    sa1.sa_handler = handleSigusr1;
    sigemptyset(&sa1.sa_mask);
    sa1.sa_flags = 0;
    sigaction(SIGUSR1, &sa1, NULL);

    sa2.sa_handler = handleSigusr2;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;
    sigaction(SIGUSR2, &sa2, NULL);

    printf("Program running. Send SIGUSR1 or SIGUSR2...\n");

    while (1) {
        pause();
        if (sigusr1Flag) {
            printf("User-defined signal 1 received\n");
            sigusr1Flag = 0;
        }

        if (sigusr2Flag) {
            printf("User-defined signal 2 received\n");
            sigusr2Flag = 0;
        }
    }
    return 0;
}