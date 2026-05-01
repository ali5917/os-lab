// 5. Mutexes (Mutual Exclusion)

// Used to prevent race conditions when multiple threads access shared resources.

// Declaration: 
pthread_mutex_t mutex;

// Initialization: 
pthread_mutex_init(&mutex, NULL);
// or 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Locking: 
pthread_mutex_lock(&mutex);
// Critical section (access shared variables here)

// Unlocking: 
pthread_mutex_unlock(&mutex);

// Destruction: 
pthread_mutex_destroy(&mutex); 
// Call when done with the mutex