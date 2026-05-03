// POSIX Threads Syntax & Notes

// Compilation: gcc yourprogram.c -o yourprogram -pthread

// 1. Thread Creation & Basic Functions

pthread_create(&thread_id, &attributes, function, arg);
// thread_id: Pointer to pthread_t variable to store the new thread's ID.
// attributes: Pointer to pthread_attr_t. Pass NULL for defaults.
// function: Function to execute. Must be: void* func(void* arg)
// arg: (void *) argument passed to the function. Pass NULL if not needed.

// Example: 
pthread_create(&thread, NULL, function, NULL);

pthread_join(thread_id, &retval);
// Waits for the specified thread to terminate.
// thread_id: ID of the thread to wait for.
// retval: Pointer to store the exit status (value from pthread_exit or return). Pass NULL if not needed.
// Example: 
pthread_join(thread, NULL);

pthread_self();
// Returns the ID of the calling thread.
// Example: 
printf("Thread ID: %lu\n", pthread_self());


// 2. Thread Termination

pthread_exit(retval);
// Terminates the calling thread and returns a value to the thread joining it.
// Note: Return heap variables (malloc), NOT stack variables!s

pthread_cancel(thread_id);
// Sends a cancellation request to a thread (doesn't kill instantly).

pthread_detach(thread_id);
// Marks the thread as detached. Its resources will be released automatically upon termination.
// Detached threads cannot be joined.


// 3. Passing Arguments & Returning Values

// Passing multiple arguments using a struct:
struct ThreadArgs {
    int id;
    char *message;
};

struct ThreadArgs *args = malloc(sizeof(struct ThreadArgs));
args->id = 1;
args->message = "Hello";
pthread_create(&thread, NULL, func, (void*) args);

// Inside the thread function (Retrieving args & Returning a value):
void* func(void* arg) {
    // cast argument back to struct
    struct ThreadArgs *my_args = (struct ThreadArgs*) arg;
    printf("Thread %d: %s\n", my_args->id, my_args->message);
    
    // Return a value using heap
    int *result = malloc(sizeof(int));
    *result = 42;
    pthread_exit(result); // or return result;
}

// In main() (Getting the return value):
void *res;
pthread_join(thread, &res);
printf("Result: %d\n", *(int*)res);
free(res); // Don't forget to free the malloc'd memory

// 4. Thread Attributes 

// Modifying thread behavior before creation (e.g., creating it as detached).

pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
pthread_create(&thread, &attr, func, NULL);
pthread_attr_destroy(&attr); // safe to destroy after creation