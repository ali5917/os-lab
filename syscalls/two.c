// Write a program that prints the current process ID (PID) and the parent process ID (PPID) 
// Implement a function to retrieve and print the user ID (UID) of the current user executing the program

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

// Function to print User ID
void printUID() {
    uid_t uid = getuid();
    printf("User ID (UID): %d\n", uid);
}

int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();

    printf("Process ID (PID): %d\n", pid);
    printf("Parent Process ID (PPID): %d\n", ppid);

    // Call function to print UID
    printUID();

    return 0;
}