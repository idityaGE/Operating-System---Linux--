#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 1
#define NUM_CONSUMERS 3

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
    .count = 0};

void *producer(void *arg) {
  int id = *((int *)arg);
  int item;
  for (int i = 0; i < 10; i++) {
    item = rand() % 100;
    pthread_mutex_lock(&buffer.mutex);

    while (buffer.count == BUFFER_SIZE) {
      printf("Producer %d : buffer full, waiting ...\n", id);
      pthread_cond_wait(&buffer.not_full, &buffer.mutex);
    }

    buffer.buffer[buffer.in] = item;
    buffer.in = (buffer.in + 1) % BUFFER_SIZE;
    buffer.count++;
    printf("Producer %d : inserted item %d\n", id, item);

    pthread_cond_signal(&buffer.not_empty);
    pthread_mutex_unlock(&buffer.mutex);
    sleep(rand() % 3);
  }

  return NULL;
}

void *consumer(void *arg) {
  int id = *((int *)arg);
  int item;
  for (int i = 0; i < 3; i++) {
    pthread_mutex_lock(&buffer.mutex);

    while (buffer.count == 0) {
      printf("Consumer %d : buffer empty, waiting...\n", id);
      pthread_cond_wait(&buffer.not_empty, &buffer.mutex);
    }

    item = buffer.buffer[buffer.out];
    buffer.out = (buffer.out + 1) % BUFFER_SIZE;
    buffer.count--;

    printf("Consumer %d : removed item %d\n", id, item);

    pthread_cond_signal(&buffer.not_full);
    pthread_mutex_unlock(&buffer.mutex);
    sleep(rand() % 3);
  }

  return NULL;
}

int main() {
  pthread_t prod_thread[NUM_PRODUCERS];
  pthread_t cons_thread[NUM_CONSUMERS];
  int prod_ids[NUM_PRODUCERS];
  int cons_ids[NUM_CONSUMERS];

  srand(time(NULL));

  pthread_mutex_init(&buffer.mutex, NULL);
  pthread_cond_init(&buffer.not_empty, NULL);
  pthread_cond_init(&buffer.not_full, NULL);

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    prod_ids[i] = i + 1;
    pthread_create(&prod_thread[i], NULL, producer, &prod_ids[i]);
  }
  for (int i = 0; i < NUM_CONSUMERS; i++) {
    cons_ids[i] = i + 1;
    pthread_create(&cons_thread[i], NULL, consumer, &cons_ids[i]);
  }

  for (int i = 0; i < NUM_PRODUCERS; i++)
    pthread_join(prod_thread[i], NULL);
  for (int i = 0; i < NUM_CONSUMERS; i++)
    pthread_join(cons_thread[i], NULL);

  pthread_mutex_destroy(&buffer.mutex);
  pthread_cond_destroy(&buffer.not_empty);
  pthread_cond_destroy(&buffer.not_full);

  return 0;
}
