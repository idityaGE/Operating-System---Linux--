#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Mutexes : [Mutual Exclusion]
 * it help to synchronization of the resource from diffrent process or thread.
 * In simple term it prevent the data being modified the there is mutex lock and 
 * it will block the other to lock the mutex when it's already locked.
 * this used to prevent from race conditions.
 */

/**
 * Race Condition : When multiple threads access shared data simultaneously
 * Critical Sections: Parts of code that should not be executed by multiple threads simultaneously
 */

typedef struct {
  int counter;
  pthread_mutex_t mutex;
} shared_data;

void *increment_counter(void *arg) {
  shared_data *data = (shared_data *)arg;

  for (int i = 0; i < 5; i++) {

    // Lock the mutex before accessing the counter
    pthread_mutex_lock(&data->mutex);

    // Critical section: safely increment the counter
    data->counter++;
    printf("Thread %lu: counter = %d\n", pthread_self(), data->counter);

    // Unlock the mutex after accessing the counter
    pthread_mutex_unlock(&data->mutex);
    sleep(1);
  }

  return NULL;
}

int main() {
  shared_data data;
  data.counter = 0;

  // initialize the mutex
  pthread_mutex_init(&data.mutex, NULL);

  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, increment_counter, &data);
  pthread_create(&thread2, NULL, increment_counter, &data);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("Final counter value: %d\n", data.counter);

  // destroy the mutex
  pthread_mutex_destroy(&data.mutex);

  return 0;
}


// adii@Virus:/mnt/d/Github/Operating System [ Linux ]/System Calls/thread$ ./mutex 
// Thread 139932050155200: counter = 1
// Thread 139932058547904: counter = 2
// Thread 139932050155200: counter = 3
// Thread 139932058547904: counter = 4
// Thread 139932058547904: counter = 5
// Thread 139932050155200: counter = 6
// Thread 139932058547904: counter = 7
// Thread 139932050155200: counter = 8
// Thread 139932058547904: counter = 9
// Thread 139932050155200: counter = 10
// Final counter value: 10
