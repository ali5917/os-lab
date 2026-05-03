// Multithreaded E-Commerce Revenue Calculator
// Each thread computes a subtotal of its assigned orders.
// Main thread sums all subtotals for final revenue.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    double *orders;
    int startIndex;
    int endIndex;
    double subTotal;
} threadArgs;

void *calcSubTotal (void *arg) {
    threadArgs *data = (threadArgs *)arg;
    int start = data->startIndex, end = data->endIndex;
    for (int i = start; i < end; i++) {
        data->subTotal += data->orders[i];   
    }

    pthread_exit(0);
}

#define numThreads 4
#define totalOrders 20

int main () {
    double orders[totalOrders] = { 
        120.50, 340.00, 89.99,  215.75, 450.00,
        60.25,  180.00, 95.50,  310.00, 75.00,
        540.00, 200.00, 135.75, 410.50, 99.99,
        260.00, 320.00, 150.25, 480.00, 110.00
    };

    pthread_t threads[numThreads];
    threadArgs args[numThreads];
    int ordersPerThread = totalOrders / numThreads;
    int extra = totalOrders % numThreads;

    for (int i = 0; i < numThreads; i++) {
        int start = i * ordersPerThread;
        int end;
        if (i == numThreads - 1) {
            end = start + ordersPerThread + extra;
        } else {
            end = start + ordersPerThread;
        }

        args[i].orders = orders;
        args[i].startIndex = start;
        args[i].endIndex = end;
        args[i].subTotal = 0.0;
        pthread_create(&threads[i], NULL, calcSubTotal, &args[i]);
    }

    double totalRevenue = 0.0;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d subtotal: $%.2f\n", i + 1, args[i].subTotal);
        totalRevenue += args[i].subTotal;
    }
    
    printf("\nTotal Revenue: $%.2f\n", totalRevenue);
    return 0;
}   