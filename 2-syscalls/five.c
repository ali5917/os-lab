// Invoke at least 4 commands from your programs, such as Cp, mkdir, rmdir, etc

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void run_command(char *cmd, char *arg1, char *arg2) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    
    if (pid == 0) {
        // Child process executes command
        if (arg2 != NULL)
            execlp(cmd, cmd, arg1, arg2, NULL);
        else if (arg1 != NULL)
            execlp(cmd, cmd, arg1, NULL);
        else
            execlp(cmd, cmd, NULL);
        
        perror("exec failed");
        exit(1);
    } else {
        // Parent waits for child to finish
        wait(NULL);
    }
}

int main() {
    run_command("mkdir", "test_dir", NULL);
    run_command("cp", "input.txt", "test_dir/");
    run_command("ls", "test_dir", NULL);
    run_command("rm", "test_dir/input.txt", NULL);
    run_command("rmdir", "test_dir", NULL);

    return 0;
}