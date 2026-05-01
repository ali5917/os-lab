// Create a program that reads input from a text file named "input.txt" 
// and writes the content to another text file named "output.txt". 
// Ensure proper error handling

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    int fd_in, fd_out;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    fd_in = open("input.txt", O_RDONLY);
    if (fd_in < 0) {
        perror("Error opening input.txt");
        exit(1);
    }

    // Open output.txt for writing (create if doesn't exist, truncate if exists)
    fd_out = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd_out < 0) {
        perror("Error opening output.txt");
        close(fd_in);
        exit(1);
    }

    // Read from input.txt and write to output.txt
    while ((bytes_read = read(fd_in, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(fd_out, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to output.txt");
            close(fd_in);
            close(fd_out);
            exit(1);
        }
    }

    if (bytes_read < 0) {
        perror("Error reading input.txt");
    }

    // Close files
    close(fd_in);
    close(fd_out);

    printf("File copied successfully.\n");
    return 0;
}