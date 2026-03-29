// Write a C program where the parent process writes three integers ('10', '2', '7') to a file 'data.txt'. 
// Use sequential forking to create three child processes: 
// Child 1 reads the first Integer, computes the sum of the first 'n' natural numbers, and prints the result.
// Child 2 reads the second Integer, computes its cube, and prints the result.
// Child 3 reads the third integer, computes its square, and prints the result.
// Ensure the parent waits for each child to complete before forking the next. 

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

int main () {
    FILE *ptr = fopen("data.txt", "w");
    if (ptr == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    if (fprintf(ptr, "10 2 7") == -1) {
        perror("fprintf");
        exit(EXIT_FAILURE);
    }

    fclose(ptr);

    for (int i = 0; i < 3; i++) { 
        pid_t pid = fork();
        if (pid == 0) {
            FILE *ptr = fopen("data.txt", "r");
            if (ptr == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            
            if (i == 0) {
                char s1[20], s2[20], s3[20];
                if (fscanf(ptr, "%s %s %s", s1, s2, s3) != 3) {
                    perror("fscanf");
                }
                int first = atoi(s1);
                int sum = 0;
                for (int j = 1; j <= first; j++) {
                    sum += j;
                }

                printf("Sum of first %d natural numbers: %d\n", first, sum);    

            } else if (i == 1) {
                char s1[20], s2[20], s3[20];
                if (fscanf(ptr, "%s %s %s", s1, s2, s3) != 3) {
                    perror("fscanf");
                }
                int second = atoi(s2);
                printf("Cube of %d: %d\n", second, second*second*second); 
            } else if (i == 2) {
                char s1[20], s2[20], s3[20];
                if (fscanf(ptr, "%s %s %s", s1, s2, s3) != 3) {
                    perror("fscanf");
                }
                int third = atoi(s3);
                printf("Square of %d: %d\n", third, third*third); 
            }
            exit(0);
        } else {
            wait(NULL);
        }
    }
    return 0;
}