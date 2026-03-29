// Create a program named stat (nano stat.c) that takes an
// integer array as command line argument, deliminated by some character such as $ (./stat 10$20$30$40)
// The program then creates 3 child processes each of which does exactly one task from the following: 
//     1) Adds them and print the result on the screen. (done by child 1)
//     2) Shows the average on the screen. (done by child 2), 
//     3) Prints the maximum number on the screen. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s num1$num2$num3...\n", argv[0]);
        return 1;
    }

    // parse input string
    int arr[100], n = 0;
    char *token;
    token = strtok(argv[1], "$");

    while (token != NULL) {
        arr[n] = atoi(token);
        n++;
        token = strtok(NULL, "$");
    }

    // create 3 child processes
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            if (i == 0) {
                // Sum
                int sum = 0;
                for (int j = 0; j < n; j++)
                    sum += arr[j];
                printf("Sum = %d\n", sum);
            }
            else if (i == 1) {
                // Average
                int sum = 0;
                for (int j = 0; j < n; j++)
                    sum += arr[j];
                printf("Average = %.2f\n", (float)sum / n);
            }
            else if (i == 2) {
                // Maximum
                int max = arr[0];
                for (int j = 1; j < n; j++) {
                    if (arr[j] > max)
                        max = arr[j];
                }
                printf("Max = %d\n", max);
            }

            exit(0);
        }
    }

    // parent waits
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}