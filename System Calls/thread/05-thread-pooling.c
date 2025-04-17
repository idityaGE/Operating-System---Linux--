#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 5

void *task(void *arg) {
  printf("Thread %ld is processing task %d\n", pthread_self(), *(int *)arg);
  sleep(1);
  return NULL;
}

int main() {
  pthread_t threads[NUM_THREADS];
  int taskNumber[NUM_THREADS] = {1, 2, 3, 4, 5};

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, task, &taskNumber[i]);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
