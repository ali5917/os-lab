// Divide a large text into segments and use multiple threads to search
// for a keyword. Each thread counts occurrences in its segment, including
// overlaps to handle boundary-spanning matches. Main thread combines results.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>

#define NUMTHREADS 5

typedef struct threadData {
	char* filename;
	char* keyword;
	int startByte;
	int endByte;
	int instances;
} threadData;

void* findInstances(void* arg) {
	threadData* data = (threadData *)arg;
	int fd = open(data->filename, O_RDONLY);
	if ( fd < 0 ) {
		perror("file open failed\n");
		pthread_exit(NULL);
	}

	int k = strlen(data->keyword);
	char* str = malloc((sizeof(char) * k) + 1);

	for(int i = data->startByte; i + k <= data->endByte; i++) {
		lseek(fd, i, SEEK_SET);
		read(fd, str, k);
		str[k] = 0;
		if (strcmp(str, data->keyword) == 0) {
			data->instances++;
		}
	}
	close(fd);
	free(str);
	pthread_exit(NULL);
}

int main() {
	char* filename = "data.txt";
	char* keyword = "login";

	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror("Couldn't open file\n");
		return -1;
	}

	int fileSize = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	int segmentSize = fileSize / NUMTHREADS;
	int remainder = fileSize % NUMTHREADS;

	int k = strlen(keyword);

	threadData data[NUMTHREADS];
	pthread_t threads[NUMTHREADS];

	for(int i = 0; i < NUMTHREADS; i++) {
		int startByte = i * segmentSize;
		int endByte = (i + 1) * segmentSize;

		if (i != NUMTHREADS - 1) {
			endByte += k;
		} else {
			endByte += remainder;
		}

		data[i].filename = filename;
		data[i].keyword = keyword;
		data[i].startByte = startByte;
		data[i].endByte = endByte;
		data[i].instances = 0;
		pthread_create(&threads[i], NULL, findInstances, &data[i]);
	}
    
	int total = 0;
	for (int i = 0; i < NUMTHREADS; i++) {
		pthread_join(threads[i], NULL);
		printf("Thread %d found %d instances\n", i, data[i].instances);
		total += data[i].instances;
	}
	printf("Total instances: %d\n", total);
}