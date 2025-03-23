#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
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
    .not_empty = PTHREAD_COND_INITIALIZER};

void *producer(void *arg) {
  int item;

  for (int i = 0; i < 10; i++) {
    item = rand() % 100;  // Generate random item

    pthread_mutex_lock(&buffer.mutex);

    // Wait if buffer is full
    while (buffer.count == BUFFER_SIZE) {
      printf("Producer: buffer full, waiting...\n");
      pthread_cond_wait(&buffer.not_full, &buffer.mutex);  // pthread_cond_wait() makes the thread wait until the condition is met.
    }

    // Add item to buffer
    buffer.buffer[buffer.in] = item;
    buffer.in = (buffer.in + 1) % BUFFER_SIZE;
    buffer.count++;

    printf("Producer: inserted item %d\n", item);

    // Signal that buffer is not empty
    pthread_cond_signal(&buffer.not_empty);  // pthread_cond_signal() wakes up the waiting thread.
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
