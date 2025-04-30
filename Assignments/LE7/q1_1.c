#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
  int counter;
  pthread_mutex_t mutex;
} shared_data;

void *lock_and_hold(void *arg) {
  shared_data *data = (shared_data *)arg;

  printf("Thread %lu: Attempting to lock the mutex\n", pthread_self());
  pthread_mutex_lock(&data->mutex);

  printf("Thread %lu: Got the lock, holding for 5 seconds...\n", pthread_self());
  data->counter++;
  sleep(5);

  printf("Thread %lu: Unlocking the mutex\n", pthread_self());
  pthread_mutex_unlock(&data->mutex);

  return NULL;
}

void *try_lock_repeatedly(void *arg) {
  shared_data *data = (shared_data *)arg;

  for (int i = 0; i < 10; i++) {
    printf("Thread %lu: Attempt %d to acquire the lock using trylock\n",
           pthread_self(), i + 1);

    if (pthread_mutex_trylock(&data->mutex) == 0) {
      // Got the lock successfully
      printf("Thread %lu: Successfully acquired the lock!\n", pthread_self());
      data->counter += 10;
      printf("Thread %lu: Counter value: %d\n", pthread_self(), data->counter);
      pthread_mutex_unlock(&data->mutex);
      printf("Thread %lu: Lock released\n", pthread_self());
      return NULL;
    } else
      // Failed to get the lock
      printf("Thread %lu: Lock acquisition failed (mutex is busy)\n", pthread_self());
    sleep(1);
  }

  printf("Thread %lu: Gave up after 10 attempts\n", pthread_self());
  return NULL;
}

int main() {
  shared_data data;
  data.counter = 0;
  pthread_mutex_init(&data.mutex, NULL);

  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, lock_and_hold, &data);

  sleep(1);

  pthread_create(&thread2, NULL, try_lock_repeatedly, &data);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("Final counter value: %d\n", data.counter);
  pthread_mutex_destroy(&data.mutex);

  return 0;
}
