// Write a program that accepts a list of integers as command line arguments 
// sorts the integers and print the sorted integers on the screen

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Error: No input provided\n");
        return 1;
    }

    int n = argc - 1;
    int *arr = malloc(n * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        char *end;
        int num = strtol(argv[i], &end, 10);

        if (*end != '\0') {
            printf("Invalid input");
            free(arr);
            return 1;
        }

        arr[i - 1] = num;
    }

    // Bubble Sort
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    // Print sorted array
    printf("Sorted integers: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}