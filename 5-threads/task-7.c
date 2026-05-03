// Each file should be assigned to a separate thread, which
// reads the file and counts the words. The main thread will collect the word
// counts from all threads and display the results for each file as well as the
// total word count across all files.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

#define NUMTHREADS 4

typedef struct threadData {
	char* filename;
	int instances;
} threadData;

void* findInstances(void* arg) {
	threadData* data = (threadData *)arg;
	int fd = open(data->filename, O_RDONLY);
	if (fd < 0 ) {
		perror("file open failed\n");
		pthread_exit(NULL);
	}

	char ch;
    int inword = 0;
    lseek(fd, 0, SEEK_SET);

    while ((read(fd, &ch, 1)) > 0) {
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == ',' || ch == '.' || ch == '!' || ch == '?' || ch == ';' || ch == ':') {
            inword = 0;
        } 
        else {
            if (!inword) {
                data->instances++;
                inword = 1;
            }
        }
    }
	close(fd);
	pthread_exit(NULL);
}

int main() {
	char* filenames[NUMTHREADS] = {"file1.txt", "file2.txt", "file3.txt", "file4.txt"};

	pthread_t threads[NUMTHREADS];
	threadData data[NUMTHREADS];

	for(int i = 0; i < NUMTHREADS; i++) {
		data[i].filename = filenames[i];
		data[i].instances = 0;
		pthread_create(&threads[i], NULL, findInstances, &data[i]);
	}

	int total = 0;
	for (int i = 0; i < NUMTHREADS; i++) {
		pthread_join(threads[i], NULL);
		printf("Thread %d found %d words from %s\n", i, data[i].instances, data[i].filename);
		total += data[i].instances;
	}
	printf("Total words: %d\n", total);
}