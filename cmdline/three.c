// Write a program that reads file in which there are integers related to series 
// store them in array than compute the missing element from that series 
// output that missing element to file

#include <stdio.h>
#include <stdlib.h>

int main() {

    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        printf("Error opening input file\n");
        return 1;
    }

    int arr[100], n = 0;
    int num, sum = 0;

    while (fscanf(fp, "%d", &num) == 1) {
        arr[n] = num;
        sum += num;
        n++;
    }
    fclose(fp);

    int min = arr[0], max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    int expected = (max - min + 1) * (min + max) / 2;
    int missing = expected - sum;

    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Error opening output file\n");
        return 1;
    }

    fprintf(out, "Missing number: %d\n", missing);
    fclose(out);

    printf("Missing number written to output.txt\n");

    return 0;
}