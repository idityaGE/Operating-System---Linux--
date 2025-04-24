#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREAD 5

void *do_something(void *arg) {
  int *id = (int *)arg;
  printf("Thread %d running.\n", *id);
  sleep(*id);
  printf("Thread %d done.\n", *id);
  return (void *)id;
}

int main() {
  pthread_t threads[NUM_THREAD];
  int thread_ids[NUM_THREAD];
  for(int i = 0; i < NUM_THREAD; i++) 
    thread_ids[i] = i + 1; 

  for(int i = 0; i < NUM_THREAD; i++) {
    if (pthread_create(&threads[i], NULL, do_something, &thread_ids[i]) != 0) {
      perror("Failed to create thread");
      exit(1);
    }
  }

  for(int i = 0; i < NUM_THREAD; i++) {
    pthread_join(threads[i], NULL);
    printf("Thread %d completed.\n", i+1);
  }

  printf("All threads have finished.\n");
  return 0;
}
