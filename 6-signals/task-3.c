// Creates a child process using fork()
// The child process runs an infinite loop printing "Child process running..."
// The parent process waits for 5 seconds and then sends a SIGTERM signal to the child
// The child process should handle the signal and print "Child terminated by parent"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile sig_atomic_t terminate = 0;

void handleSigterm(int sig) {
    terminate = 1;
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    // child process
    if (pid == 0) {
        struct sigaction sa;
        sa.sa_handler = handleSigterm;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        
        sigaction(SIGTERM, &sa, NULL);

        while (!terminate) {
                printf("Child process running...\n");
            sleep(1);
        }

        printf("Child terminated by parent\n");
        exit(0);
    }

    // parent process
    else {
        sleep(5);

        printf("Parent sending SIGTERM to child...\n");
        kill(pid, SIGTERM);

        // Wait for child to finish
        wait(NULL);
        printf("Parent exiting...\n");
    }
    return 0;
}