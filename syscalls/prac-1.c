// Your program should perform the following tasks:
// The parent process should display the list of all files and directories in the current working directory.
// The child process should display the absolute path of the current working directory.

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
        char s[100];
        printf("The absolute path of the working directory:\n");
        execlp("realpath", "realpath", ".", NULL);
        perror("execlp realpath"); 
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        printf("\nList of files in current directory:\n");
        execlp("ls", "ls", NULL);
        perror("execlp ls"); 
        exit(EXIT_FAILURE);
    }
}