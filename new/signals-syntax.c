// 1. signal()
// Syntax: signal(signal_name, handler_function);

// Example: 
signal(SIGINT, my_handler);

// The handler function must:
// - Return void
// - Take one int parameter (the signal number)
void my_handler(int signum) {
    // signum is the signal number that was received (e.g., SIGINT = 2)
}

// Special Handler Options:
signal(SIGINT, my_handler); // Run custom function
signal(SIGINT, SIG_IGN);    // Ignore the signal
signal(SIGINT, SIG_DFL);    // Restore default behavior 

// Full Minimal Example (signal)
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum) {
    printf("Caught signal %d\n", signum);
}

int main_legacy() {
    signal(SIGINT, handler);
    while(1) { 
        sleep(1); 
    }
    return 0;
}


// 2. sigaction() 

struct sigaction sa;
sa.sa_handler = handler;      // The function to run
sigemptyset(&sa.sa_mask);     // Signals to block DURING handler execution
sa.sa_flags = SA_RESTART;     // Behavior flags (SA_RESTART restarts interrupted syscalls)

sigaction(SIGINT, &sa, NULL); 
// Arguments: (signal_num, &new_action, &old_action_to_save)

// Full Minimal Example (sigaction)
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sa_handler_func(int signum) {
    printf("Caught signal %d\n", signum);
}

int main_modern() {
    struct sigaction sa;
    
    sa.sa_handler = sa_handler_func;
    sigemptyset(&sa.sa_mask);    // while handling this signal, don’t block any additional signals.
    sa.sa_flags = SA_RESTART;    // restart interrupted reads/waits automatically

    sigaction(SIGINT, &sa, NULL);

    while(1) { sleep(1); }
    return 0;
}


// Comparison: signal vs sigaction

// -----------------------------------------------------------------------------
// | FEATURE                | signal()               | sigaction()             |
// -------------------------|------------------------|--------------------------
// | Syntax                 | 1 line                 | 4 lines + struct        |  
// | Reliability            | Unpredictable          | Reliable/POSIX Standard |
// | Handler Reset?         | Sometimes (System V)   | No (Stays registered)   |
// | Block other signals?   | No control             | Yes (via sa_mask)       |
// | Restart Syscalls?      | No control             | Yes (via SA_RESTART)    |
// -----------------------------------------------------------------------------


// Other Important Signal Functions

// pause(): Suspend process until ANY signal is received
pause();
// Note:
// Use pause() when the only purpose of that line is to wait for a signal — nothing else. 
// Use sleep() when the loop is also doing work (printing, checking flags, etc.)

// raise(): Send a signal to YOURSELF
raise(SIGINT);

// kill(): Sends a signal to a specific process using its PID. 
// Can be used to terminate (SIGTERM/SIGKILL) or notify (SIGUSR1) other processes.
kill(pid, SIGTERM);
kill(getpid(), SIGUSR1); // Sending to yourself is the same as raise()

// pthread_kill(): Like kill(), but targets a specific THREAD within your process 
// instead of the whole process.
pthread_kill(thread_id, SIGUSR1);

// pthread_sigmask(): Used by a thread to block/unblock specific signals.
sigset_t mask;
sigemptyset(&mask);              // 1. Initialize an empty set
sigaddset(&mask, SIGUSR1);       // 2. Add SIGUSR1 to the set
pthread_sigmask(SIG_BLOCK, &mask, NULL); // 3. Tell the OS to block signals in this set for this thread

// Glossary of Terms
// sigset_t    : A data type representing a "set" or "collection" of signals.
// sigemptyset : Clears the signal set (makes it empty).
// sigaddset   : Adds a specific signal to the set.
// SIGUSR1     : A "User-Defined" signal for custom programming logic.
// SIG_BLOCK   : A flag that says "add these signals to the current blocked list."
// SIG_UNBLOCK : A flag that says "remove these signals from the blocked list."

// How Blocking Works:
// - Blocking is NOT "Ignoring." 
// - If a signal is blocked, it becomes "PENDING" (it waits in a queue).
// - Once you UNBLOCK it, the signal immediately fires.
// - pthread_sigmask() allows one specific thread to decide which signals 
//   it is willing to be interrupted by.