// Write a program that takes some integers in the form of series as command line parameters
// store them in array than compute the missing element from that series
// output that missing element to file.
.

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Error: Provide at least 2 numbers\n");
        return 1;
    }

    int n = argc - 1;
    int *arr = malloc(n * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    int sum = 0;

    for (int i = 1; i < argc; i++) {
        char *end;
        int num = strtol(argv[i], &end, 10);

        if (*end != '\0') {
            printf("Invalid input: %s\n", argv[i]);
            free(arr);
            return 1;
        }

        arr[i - 1] = num;
        sum += num;
    }

    int min = arr[0], max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    int expected = (max - min + 1) * (min + max) / 2;       // n * (first + last) / 2

    int missing = expected - sum;

    FILE *fp = fopen("output.txt", "w");
    if (fp == NULL) {
        printf("File error\n");
        free(arr);
        return 1;
    }

    fprintf(fp, "Missing number: %d\n", missing);
    fclose(fp);

    printf("Missing number written to file\n");

    free(arr);
    return 0;
}