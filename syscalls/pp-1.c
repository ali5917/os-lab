// Write a C program that performs the following operations:
// The parent process reads an integer N from the command line and passes it to the child process.
// The child process computes and prints the sum S(N) of the reciprocals of the first N even numbers.
// The parent process waits for the child process to complete before terminating.
// Implement error handling to manage invalid inputs (e.g., non-integer or negative values).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./%s N", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *end;
    long N = strtol(argv[1], &end, 10);

    if (*end != '\0' || N <= 0) {
        fprintf(stderr, "Number must be a positive integer!\n");
        exit(EXIT_FAILURE);
    }
    
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork ();
    
    if (pid == 0) {
        close(fd[1]);
        int n;
        ssize_t bytesRead = read(fd[0], &n, sizeof(int));
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        close(fd[0]);

        double sum = 0.0;
        for (int i = 1; i <= n; i++) {
            sum += 1.0 / (2 * i);
        }
        printf("Sum: %.6f\n", sum);

    } else {
        close (fd[0]);
        int n = (int) N;
        ssize_t bytesWritten = write(fd[1], &n, sizeof(int));
        if (bytesWritten == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(fd[1]);

        wait(NULL);
    }
}