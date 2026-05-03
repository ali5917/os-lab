// Multithreaded Server Log Line Counter
// Each thread reads its assigned portion of the file and counts lines.
// Main thread aggregates the total.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define numThreads 4
#define logFile "data.txt"

typedef struct {
    long start_byte;   // byte offset where this thread starts reading 
    long end_byte;     // byte offset where this thread stops reading  
    int  line_count;   // result: number of lines counted              
} ThreadArgs;

void *countLines(void *arg) {
    ThreadArgs *data = (ThreadArgs *)arg;
    int fd = open(logFile, O_RDONLY);
    if (fd < 0) {
        perror("open");
        data->line_count = 0;
        pthread_exit(NULL);
    }
    
    int count = 0;
    char ch;
    
    lseek(fd, data->start_byte, SEEK_SET);

    long pos = data->start_byte;
    
    // skip partial line if not first thread
    if (data->start_byte != 0) {
        while (read(fd, &ch, 1) > 0 && ch != '\n') {
            pos++;
        }
    }

    // start counting full lines 
    while (pos < data->end_byte && read(fd, &ch, 1) > 0) {
        if (ch == '\n') {
            count++;
        }
        pos++;
    }

    close(fd);
    data->line_count = count;
    pthread_exit(NULL);
}

int main() {
    // find file size 
    int fd = open(logFile, O_RDONLY);
    long fileSize = lseek(fd, 0, SEEK_END);
    close(fd);

    // divide file into equal byte chunks 
    pthread_t threads[numThreads];
    ThreadArgs args[numThreads];
    long chunk = fileSize / numThreads;
 
    for (int i = 0; i < numThreads; i++) {
        args[i].start_byte = i * chunk;
        if (i == numThreads - 1) {
            args[i].end_byte = fileSize;
        } else {
            args[i].end_byte = (i + 1) * chunk;
        }
        pthread_create(&threads[i], NULL, countLines, &args[i]);
    }
    
    int total = 0;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        printf("Thread %d counted %d lines\n", i, args[i].line_count);
        total += args[i].line_count;
    }
    total += 1;     // last line with no \n

    printf("\nTotal requests (lines): %d\n", total);
    return 0;
}