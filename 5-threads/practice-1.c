// Practice Task: Parallel File Search
// Write a multithreaded C program to count the occurrences of a specific target keyword in a file.
// 1. Assume a text file "data.txt" exists. The main thread should determine its total size in bytes.
// 2. Ask the user for a "target" keyword to search for.
// 3. Create 4 threads. Divide the file equally among them by byte chunks.
// 4. Pass a struct to each thread containing the filename, the target keyword, its start_byte, 
//    and its end_byte.
// 5. Each thread should open the file, seek to its start_byte, count how many times the target keyword 
//    appears in its chunk, and return that local count using pthread_exit.
// 6. The main thread should join all threads, retrieve their local counts, 
//    calculate the total occurrences, and print the final result.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

#define dataFile "data.txt"
#define numThreads 4

typedef struct {
    char *filename;
    char *keyword;
    long start;
    long end;
    int count;
} Data;

void* countKey (void* arg) {
    Data *data = (Data*) arg; 
    int fd = open(data->filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        pthread_exit(NULL);
    }

    int k = strlen(data->keyword);
    char *temp = malloc(k + 1);
    
    for (int i = data->start; i + k <= data->end; i++) {
        lseek(fd, i, SEEK_SET);
        read(fd, temp, k);
        temp[k] = '\0';
        if (strcmp(temp, data->keyword) == 0) {
            data->count++;
        }
    }

    close(fd);
    free(temp);
    pthread_exit(0);
}

int main () {
    char keyword[100];
    printf("Keyword: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0; 

    int fd = open(dataFile, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open data.txt");
        return 1;
    }
    long fileSize = lseek(fd, 0, SEEK_END);
    int chunk = fileSize / numThreads;
    close(fd);

    pthread_t threads[numThreads];
    Data args[numThreads];

    int k = strlen(keyword);

    for (int i = 0; i < numThreads; i++) {
        long startByte = i * chunk;
        long endByte;

        if (i == numThreads - 1) {
            endByte = fileSize;
        } else {
            endByte = ((i + 1) * chunk) + k;
        }

        args[i].filename = dataFile;
        args[i].keyword = keyword;
        args[i].start = startByte;
        args[i].end = endByte;
        args[i].count = 0;
        pthread_create(&threads[i], NULL, countKey, &args[i]);
    }

    int totalCount = 0;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        totalCount += args[i].count;
    }

    printf("Total instances: %d\n", totalCount);
    return 0;
}