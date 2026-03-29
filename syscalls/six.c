// Write a program that initially set an alarm for 5 seconds 
// Enter a sleep loop. Within this loop, the program should print a message every second. 
// When the alarm signal is received after 5 seconds, the program should terminate

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void alarm_handler(int sig) {
    printf("\nAlarm received! Terminating program.\n");
    exit(0);
}

int main() {
    // set up the signal handler
    signal(SIGALRM, alarm_handler);

    alarm(5);

    int count = 0;
    while (1) {
        printf("Sleeping... %d second(s) passed\n", count++);
        sleep(1); 
    }

    return 0;
}