//  Create multiple threads to compute average, max, min, and pass count 
//  from a list of student marks, return results to main, and display them.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMSTUDENTS 5
#define PASS_MARKS 50

// Average
void* calcAverage(void* arg) {
    int* marks = (int*) arg;

    float* avg = malloc(sizeof(float));
    *avg = 0;

    for (int i = 0; i < NUMSTUDENTS; i++) {
        *avg += marks[i];
    }

    *avg /= NUMSTUDENTS;

    pthread_exit(avg);
}

// Maximum
void* findMax(void* arg) {
    int* marks = (int*) arg;

    int* max = malloc(sizeof(int));
    *max = marks[0];

    for (int i = 1; i < NUMSTUDENTS; i++) {
        if (marks[i] > *max) {
            *max = marks[i];
        }
    }

    pthread_exit(max);
}

// Minimum
void* findMin(void* arg) {
    int* marks = (int*) arg;

    int* min = malloc(sizeof(int));
    *min = marks[0];

    for (int i = 1; i < NUMSTUDENTS; i++) {
        if (marks[i] < *min) {
            *min = marks[i];
        }
    }

    pthread_exit(min);
}

// Pass Count
void* countPass(void* arg) {
    int* marks = (int*) arg;

    int* count = malloc(sizeof(int));
    *count = 0;

    for (int i = 0; i < NUMSTUDENTS; i++) {
        if (marks[i] >= PASS_MARKS) {
            (*count)++;
        }
    }

    pthread_exit(count);
}

int main() {
    pthread_t t1, t2, t3, t4;

    int marks[NUMSTUDENTS] = {60, 45, 75, 30, 90};

    // Create threads
    pthread_create(&t1, NULL, calcAverage, marks);
    pthread_create(&t2, NULL, findMax, marks);
    pthread_create(&t3, NULL, findMin, marks);
    pthread_create(&t4, NULL, countPass, marks);

    // Join threads and collect results
    void *res1, *res2, *res3, *res4;

    pthread_join(t1, &res1);
    pthread_join(t2, &res2);
    pthread_join(t3, &res3);
    pthread_join(t4, &res4);

    // Extract values
    float avg = *(float*)res1;
    int max = *(int*)res2;
    int min = *(int*)res3;
    int pass = *(int*)res4;

    // Print results
    printf("Average Marks: %.2f\n", avg);
    printf("Highest Marks: %d\n", max);
    printf("Lowest Marks: %d\n", min);
    printf("Passed Students: %d\n", pass);

    // Free allocated memory
    free(res1);
    free(res2);
    free(res3);
    free(res4);

    return 0;
}
