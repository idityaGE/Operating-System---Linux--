# Threads and POSIX Threads (pthreads) in C

## 1. Thread Basics

### What is a Thread?
A thread is the smallest unit of processing that can be scheduled by an operating system. Threads exist within a process and share the same memory space.

### Threads vs. Processes
- **Processes**: Have separate memory spaces, are heavyweight, switching is expensive
- **Threads**: Share memory space within a process, are lightweight, switching is efficient

## 2. POSIX Threads (pthreads)

Pthreads is a standardized API for thread creation and synchronization defined by the POSIX standard.

### Key Header File
```c
#include <pthread.h>
```

## 3. Basic Thread Operations

### Creating a Thread

```c
pthread_t thread_id;
pthread_create(&thread_id, NULL, thread_function, thread_argument);
```

### Joining a Thread
Waits for a thread to terminate:
```c
pthread_join(thread_id, &return_value);
```

### Detaching a Thread
Makes a thread run independently:
```c
pthread_detach(thread_id);
```

## 4. Thread Synchronization

### Mutexes (Mutual Exclusion)
```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&mutex);
// Critical section (protected code)
pthread_mutex_unlock(&mutex);
```

### Condition Variables
Used for threads to wait for a condition to be true:
```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_wait(&cond, &mutex);
pthread_cond_signal(&cond);
```

## 5. Practical Examples

### Basic Thread Example

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *task(void *arg) {
    int id = *((int*)arg);
    printf("Thread %d is running\n", id);
    sleep(1);
    printf("Thread %d is done\n", id);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    int id1 = 1, id2 = 2;
    
    // Create two threads
    pthread_create(&thread1, NULL, task, &id1);
    pthread_create(&thread2, NULL, task, &id2);
    
    printf("Main thread: created two threads\n");
    
    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Main thread: all threads have completed\n");
    return 0;
}
```

### Data Sharing Between Threads

```c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Shared data structure
typedef struct {
    int counter;
    pthread_mutex_t mutex;
} shared_data;

void *increment_counter(void *arg) {
    shared_data *data = (shared_data*)arg;
    
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&data->mutex);
        data->counter++;
        printf("Thread %lu: counter = %d\n", pthread_self(), data->counter);
        pthread_mutex_unlock(&data->mutex);
        sleep(1);
    }
    
    return NULL;
}

int main() {
    shared_data data;
    data.counter = 0;
    pthread_mutex_init(&data.mutex, NULL);
    
    pthread_t thread1, thread2;
    
    // Create threads that share data
    pthread_create(&thread1, NULL, increment_counter, &data);
    pthread_create(&thread2, NULL, increment_counter, &data);
    
    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    
    printf("Final counter value: %d\n", data.counter);
    pthread_mutex_destroy(&data.mutex);
    
    return 0;
}
```

### Producer-Consumer Pattern

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5

// Shared buffer
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} buffer_t;

buffer_t buffer = {
    .in = 0,
    .out = 0,
    .count = 0,
    .mutex = PTHREAD_MUTEX_INITIALIZER,
    .not_full = PTHREAD_COND_INITIALIZER,
    .not_empty = PTHREAD_COND_INITIALIZER
};

void *producer(void *arg) {
    int item;
    
    for (int i = 0; i < 10; i++) {
        item = rand() % 100;  // Generate random item
        
        pthread_mutex_lock(&buffer.mutex);
        
        // Wait if buffer is full
        while (buffer.count == BUFFER_SIZE) {
            printf("Producer: buffer full, waiting...\n");
            pthread_cond_wait(&buffer.not_full, &buffer.mutex);
        }
        
        // Add item to buffer
        buffer.buffer[buffer.in] = item;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;
        buffer.count++;
        
        printf("Producer: inserted item %d\n", item);
        
        // Signal that buffer is not empty
        pthread_cond_signal(&buffer.not_empty);
        pthread_mutex_unlock(&buffer.mutex);
        
        sleep(rand() % 3);  // Sleep for random time
    }
    
    return NULL;
}

void *consumer(void *arg) {
    int item;
    
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&buffer.mutex);
        
        // Wait if buffer is empty
        while (buffer.count == 0) {
            printf("Consumer: buffer empty, waiting...\n");
            pthread_cond_wait(&buffer.not_empty, &buffer.mutex);
        }
        
        // Remove item from buffer
        item = buffer.buffer[buffer.out];
        buffer.out = (buffer.out + 1) % BUFFER_SIZE;
        buffer.count--;
        
        printf("Consumer: removed item %d\n", item);
        
        // Signal that buffer is not full
        pthread_cond_signal(&buffer.not_full);
        pthread_mutex_unlock(&buffer.mutex);
        
        sleep(rand() % 2);  // Sleep for random time
    }
    
    return NULL;
}

int main() {
    pthread_t prod, cons;
    
    // Seed random number generator
    srand(time(NULL));
    
    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);
    
    // Wait for threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    
    return 0;
}
```

## 6. Thread-Safety Considerations

- **Race Conditions**: When multiple threads access shared data simultaneously
- **Critical Sections**: Parts of code that should not be executed by multiple threads simultaneously
- **Thread-Safe Functions**: Functions that can be safely called from multiple threads
- **Reentrant Code**: Code that can be interrupted and resumed without errors

## 7. Tips for Thread Programming

1. Minimize shared data between threads
2. Always protect shared data with synchronization primitives
3. Be aware of deadlocks when using multiple locks
4. Consider using thread pools for managing many short-lived tasks
5. Test thoroughly for race conditions

Would you like me to elaborate on any specific aspect of thread programming?
