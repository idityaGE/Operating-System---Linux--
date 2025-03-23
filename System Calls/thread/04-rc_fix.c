#include <pthread.h>
#include <stdio.h>

int counter = 0;
pthread_mutex_t lock;

/**
 * pthread_mutex_lock(&lock) ensures only one thread updates counter at a time.
 * pthread_mutex_unlock(&lock) releases the lock.
 */

void *incrementCounter(void *arg) {
  for (int i = 0; i < 1000000; i++) {
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

int main() {
  pthread_t t1, t2;
  pthread_mutex_init(&lock, NULL);

  pthread_create(&t1, NULL, incrementCounter, NULL);
  pthread_create(&t2, NULL, incrementCounter, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  pthread_mutex_destroy(&lock);
  printf("Final Counter: %d\n", counter);  // Correct result

  return 0;
}

// adii@Virus:/mnt/d/Github/Operating System [ Linux ]/System Calls/thread$ ./rc_fix
// Final Counter : 2000000
