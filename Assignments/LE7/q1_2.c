#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/**
 * The pthread_mutexattr_t and its associated functions allow you to customize mutex
 * behavior by setting various attributes. This is useful when you need mutexes with
 * special properties beyond the default behavior.
 */

/**
 * pthread_mutexattr_t - A data structure that holds mutex attributes
 * pthread_mutexattr_init() - Initializes a mutex attribute object
 * pthread_mutexattr_destroy() - Destroys a mutex attribute object when no longer needed
 */

/**
 * Type: Normal, recursive, error checking, etc.
 * Protocol: Priority inheritance, priority ceiling, etc.
 * Process sharing: Whether mutex can be shared between processes
 * Robustness: How the mutex behaves when a thread terminates while holding the lock
 */

typedef struct {
  int counter;
  pthread_mutex_t mutex;
} shared_data;

void *recursive_locking(void *arg) {
  shared_data *data = (shared_data *)arg;
  printf("Thread %lu: first lock attempt\n", pthread_self());
  pthread_mutex_lock(&data->mutex);
  data->counter++;
  printf("Thread %lu: Got first lock, counter = %d\n", pthread_self(), data->counter);

  // With a recursive mutex, this second lock will succeed
  // With a normal mutex, this would cause deadlock
  printf("Thread %lu: Second lock attempt (recursive)\n", pthread_self());
  pthread_mutex_lock(&data->mutex);
  data->counter++;
  printf("Thread %lu: Got second lock, counter = %d\n", pthread_self(), data->counter);

  // Need to unlock the same number of times as we locked
  printf("Thread %lu: First unlock\n", pthread_self());
  pthread_mutex_unlock(&data->mutex);

  printf("Thread %lu: Second unlock\n", pthread_self());
  pthread_mutex_unlock(&data->mutex);

  return NULL;
}

int main() {
  pthread_t thread;
  shared_data data = {
      .counter = 0};

  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&data.mutex, &attr);
  pthread_mutexattr_destroy(&attr);

  pthread_create(&thread, NULL, recursive_locking, &data);
  pthread_join(thread, NULL);

  printf("Final counter value: %d\n", data.counter);
  pthread_mutex_destroy(&data.mutex);
  return 0;
}
