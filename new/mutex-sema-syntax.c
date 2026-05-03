// POSIX Mutexes & Semaphores Syntax & Notes

// Compilation: gcc yourprogram.c -o yourprogram -pthread

// 1. Mutexes (Mutual Exclusion)
// Used to prevent race conditions when multiple threads access shared resources.
// Only the thread that locks the mutex can unlock it.

// Declaration: 
pthread_mutex_t mutex;

// Initialization: 
pthread_mutex_init(&mutex, NULL);
// The second argument is for mutex attributes (pass NULL for defaults).

// Locking: 
pthread_mutex_lock(&mutex);
// Acquires the lock. If the mutex is already locked by another thread, 
// the calling thread blocks until the mutex becomes available.

// Critical section (access shared variables here)

// Unlocking: 
pthread_mutex_unlock(&mutex);
// Releases the lock.

// Destruction: 
pthread_mutex_destroy(&mutex); 
// Call when done with the mutex to free associated resources.



// 2. Semaphores
// Used for signaling between threads or managing access to a pool of resources.
// Include header: #include <semaphore.h>

// Declaration:
sem_t semaphore;

// Initialization:
sem_init(&semaphore, 0, 1);
// &semaphore: pointer to the semaphore.
// 0: indicates the semaphore is shared between threads of a process (not across processes).
// 1: initial value of the semaphore (e.g., 1 for a binary semaphore, >1 for counting).

// Wait (P operation / Down):
sem_wait(&semaphore);
// Decrements the semaphore value. If the value is 0, it blocks until 
// another thread posts to the semaphore (increments it).

// Critical section or synchronized code

// Post (V operation / Up):
sem_post(&semaphore);
// Increments the semaphore value, potentially waking up a waiting thread.

// Destruction:
sem_destroy(&semaphore);
// Call when done with the semaphore to free associated resources.



// =============================================================================
// 3. Choosing Your Primitive (Mutex vs Semaphore)
// =============================================================================

// --- Real-world Scenarios ---

// Scenario A: The Studio Apartment (Single bathroom, only one person inside)
//   - Is it a simple yes/no?             Yes (Is bathroom free?)
//   - Does the same person lock/unlock?  Yes.
//   -> DECISION: Mutex

// Scenario B: The Shared Printer Pool (4 identical printers, queue of workers)
//   - Is it a simple yes/no?             No (There are 4).
//   - Is there a count involved?         Yes.
//   - Does it represent available slots? Yes.
//   -> DECISION: Counting Semaphore (initialized to 4)

// Scenario C: The "Start Race" Pistol (10 runners waiting, official fires pistol)
//   - Is it a simple yes/no?             Yes (Is the race started?)
//   - Does SAME thread lock/unlock?      No. Runners wait (locked), official signals (unlocks).
//   -> DECISION: Binary Semaphore

// Scenario D: The Popular Nightclub (Capacity of 50, track exact number dancing)
//   - Is there a count?                  Yes.
//   - Is the count the "permits"?        No, it's a variable we read/update (count++).
//   -> DECISION: Integer + Mutex
//   -> WHY? You need a plain int to store the number, and a mutex to make sure 
//           two people entering at the same time don't corrupt that integer.