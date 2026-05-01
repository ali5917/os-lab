// Your program should:
// The child process should create a new directory named TestDir.
// The parent process list all files and directories in the current location to verify that TestDir has been created.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main () {
    pid_t pid = fork ();
    if (pid == 0) {
        printf("Creating TestDir directory...\n");
        execlp("mkdir", "mkdir", "TestDir", NULL);
        perror("execlp mkdir"); 
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        printf("\nList of files in current directory:\n");
        execlp("ls", "ls", NULL);
        perror("execlp ls"); 
        exit(EXIT_FAILURE);
    }
}