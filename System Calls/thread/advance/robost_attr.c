#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t robust_mutex;

void *thread_function(void *arg) {
  printf("Thread %lu: Acquiring mutex\n", pthread_self());
  pthread_mutex_lock(&robust_mutex);
  printf("Thread %lu: Got the lock, now terminating without unlocking\n", pthread_self());

  // Terminate thread without releasing mutex
  pthread_exit(NULL);

  // This code is never reached
  pthread_mutex_unlock(&robust_mutex);
  return NULL;
}

int main() {
  pthread_t thread;
  pthread_mutexattr_t attr;

  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
  pthread_mutex_init(&robust_mutex, &attr);
  pthread_mutexattr_destroy(&attr);

  pthread_create(&thread, NULL, thread_function, NULL);
  pthread_join(thread, NULL);

  // Try to lock the mutex after thread terminated while holding it
  int lock_result = pthread_mutex_lock(&robust_mutex);
  if (lock_result == EOWNERDEAD) {
    printf("Main: Previous owner died while holding the mutex\n");
    // Mark mutex as consistent to make it usable again
    pthread_mutex_consistent(&robust_mutex);
  } else if (lock_result == 0) {
    printf("Main: Mutex acquired normally\n");
  }

  pthread_mutex_unlock(&robust_mutex);
  pthread_mutex_destroy(&robust_mutex);

  return 0;
}
