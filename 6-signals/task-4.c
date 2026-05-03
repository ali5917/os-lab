// Sets an alarm for 5 seconds using alarm()
// When the alarm signal (SIGALRM) is triggered:
// Print "Time's up!"
// The program should keep running in a loop until the alarm occurs

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t alarmTriggered = 0;

void handleSigalrm(int sig) {
    alarmTriggered = 1;
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handleSigalrm;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGALRM, &sa, NULL);

    alarm(5);

    while (!alarmTriggered) {
        printf("Waiting...\n");
        sleep(1);
    }

    printf("Time's up!\n");

    return 0;
}