// Write a C program that takes some integers as command line parameters
// Stores them in an array and prints the sum and average of that array 
// Include all possible error checks.

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("No input provided\n");
        return 1;
    }

    int n = argc - 1;
    int *arr = malloc(n * sizeof(int));
    int sum = 0;

    for (int i = 1; i < argc; i++) {
        char *end;

        int num = strtol(argv[i], &end, 10);

        if (*end != '\0') {
            printf("Error: Invalid input");
            free(arr);
            return 1;
        }

        arr[i - 1] = num;
        sum += num;
    }

    float avg = (float)sum / n;

    printf("Sum = %d\n", sum);
    printf("Average = %.2f\n", avg);

    free(arr);
    return 0;
}