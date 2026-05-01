// Write a program that creates two child processes. 
// One child print its process ID (PID), and the other child  print its parent process ID (PPID). 
// The parent wait for both child processes to terminate before ending

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            if (i == 0)
                printf("Child 1 PID: %d\n", getpid());
            else
                printf("Child 2 PPID: %d\n", getppid());

            exit(0);
        }
    }

    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }

    printf("Parent process finished.\n");
    return 0;
}