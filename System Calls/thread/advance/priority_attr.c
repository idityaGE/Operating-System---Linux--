#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t priority_mutex;

void *high_priority_thread(void *arg) {
  struct timespec ts;
  ts.tv_sec = 1;
  ts.tv_nsec = 0;

  // Give low priority thread time to acquire the lock
  nanosleep(&ts, NULL);

  printf("High priority thread: Attempting to lock mutex\n");
  // When using priority inheritance, this will boost the low priority thread
  pthread_mutex_lock(&priority_mutex);
  printf("High priority thread: Got the mutex\n");

  pthread_mutex_unlock(&priority_mutex);
  return NULL;
}

void *low_priority_thread(void *arg) {
  printf("Low priority thread: Locking mutex\n");
  pthread_mutex_lock(&priority_mutex);

  printf("Low priority thread: Holding mutex for 3 seconds\n");
  sleep(3);  // Hold the lock while high priority thread waits

  printf("Low priority thread: Unlocking mutex\n");
  pthread_mutex_unlock(&priority_mutex);
  return NULL;
}

int main() {
  pthread_t low, high;
  pthread_attr_t attr_low, attr_high;
  struct sched_param param_low, param_high;

  // Set up priority inheritance mutex
  pthread_mutexattr_t mutex_attr;
  pthread_mutexattr_init(&mutex_attr);
  pthread_mutexattr_setprotocol(&mutex_attr, PTHREAD_PRIO_INHERIT);
  pthread_mutex_init(&priority_mutex, &mutex_attr);

  // Create threads with different priorities
  pthread_attr_init(&attr_low);
  pthread_attr_init(&attr_high);

  param_low.sched_priority = 10;
  param_high.sched_priority = 20;

  pthread_attr_setschedparam(&attr_low, &param_low);
  pthread_attr_setschedparam(&attr_high, &param_high);

  // Create threads
  pthread_create(&low, &attr_low, low_priority_thread, NULL);
  pthread_create(&high, &attr_high, high_priority_thread, NULL);

  pthread_join(low, NULL);
  pthread_join(high, NULL);

  pthread_mutex_destroy(&priority_mutex);
  pthread_mutexattr_destroy(&mutex_attr);

  return 0;
}
