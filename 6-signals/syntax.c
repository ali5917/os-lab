/*
 * Q1 - Multithreaded Server Log Line Counter
 * Each thread reads its assigned portion of the file and counts lines.
 * Main thread aggregates the total.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 4
#define LOG_FILE    "server.log"

typedef struct {
    long start_byte;   /* byte offset where this thread starts reading */
    long end_byte;     /* byte offset where this thread stops reading  */
    int  line_count;   /* result: number of lines counted              */
} ThreadArgs;

void *count_lines(void *arg) {
    ThreadArgs *data = (ThreadArgs *)arg;
    FILE *fp = fopen(LOG_FILE, "r");
    if (!fp) {
        perror("fopen");
        data->line_count = 0;
        pthread_exit(NULL);
    }

    fseek(fp, data->start_byte, SEEK_SET);

    int count = 0;
    long pos   = data->start_byte;
    int  ch;

    while (pos < data->end_byte && (ch = fgetc(fp)) != EOF) {
        if (ch == '\n') count++;
        pos++;
    }

    fclose(fp);
    data->line_count = count;
    pthread_exit(NULL);
}

int main() {
    /* ── create a sample log file for demonstration ── */
    FILE *fp = fopen(LOG_FILE, "w");
    if (!fp) { perror("fopen"); return 1; }
    for (int i = 1; i <= 100; i++)
        fprintf(fp, "GET /page%d HTTP/1.1 200 OK\n", i);
    fclose(fp);

    /* ── find file size ── */
    fp = fopen(LOG_FILE, "r");
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fclose(fp);

    /* ── divide file into equal byte chunks ── */
    pthread_t   threads[NUM_THREADS];
    ThreadArgs  args[NUM_THREADS];
    long        chunk = file_size / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].start_byte = i * chunk;
        args[i].end_byte   = (i == NUM_THREADS - 1) ? file_size : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, count_lines, &args[i]);
    }

    int total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d counted %d lines\n", i, args[i].line_count);
        total += args[i].line_count;
    }

    printf("\nTotal requests (lines): %d\n", total);
    remove(LOG_FILE);
    return 0;
}