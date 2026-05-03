// Each thread processes independent numeric tasks (square, cube, factorial)
// and stores results in a shared array. Main thread waits for all threads
// and prints final results.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMTASKS 9

typedef struct {
    int number;
    int taskType;   // 0 = square, 1 = cube, 2 = factorial
    int index;      // position in shared result array
    int* results;   // shared array
} TaskData;

int factorial(int n) {
    int res = 1;
    for (int i = 1; i <= n; i++) {
        res *= i;
    }
    return res;
}

void* processTask(void* arg) {
    TaskData* data = (TaskData*) arg;

    int num = data->number;
    int result = 0;

    if (data->taskType == 0) {
        result = num * num;
    }
    else if (data->taskType == 1) {
        result = num * num * num;
    }
    else if (data->taskType == 2) {
        result = factorial(num);
    }

    data->results[data->index] = result;
    pthread_exit(NULL);
}

int main() {
    int results[NUMTASKS];

    pthread_t threads[NUMTASKS];
    TaskData data[NUMTASKS];

    int numbers[] = {2, 3, 4, 5, 6, 7, 3, 4, 5};
    int taskTypes[] = {0, 1, 2, 0, 1, 2, 2, 1, 0};

    for (int i = 0; i < NUMTASKS; i++) {

        data[i].number = numbers[i];
        data[i].taskType = taskTypes[i];
        data[i].index = i;
        data[i].results = results;

        pthread_create(&threads[i], NULL, processTask, &data[i]);
    }

    for (int i = 0; i < NUMTASKS; i++) {
        pthread_join(threads[i], NULL);
    }

    // display results
    for (int i = 0; i < NUMTASKS; i++) {
        if (taskTypes[i] == 0)
            printf("Square of %d = %d\n", numbers[i], results[i]);
        else if (taskTypes[i] == 1)
            printf("Cube of %d = %d\n", numbers[i], results[i]);
        else
            printf("Factorial of %d = %d\n", numbers[i], results[i]);
    }

    return 0;
}
