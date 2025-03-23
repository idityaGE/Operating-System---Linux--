#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Thread : thread is the smallest unit of processing that
 * can be scheduled by an operating system.
 * Thread exist within a process and share the same memory space.
 */

/**
 * Thread Vs Process
 * Processes: Have separate memory spaces, are heavyweight, switching is expensive
 * Threads: Share memory space within a process, are lightweight, switching is efficient
 */

/**
 * type : pthread_t
 *
 *  pthread_create(&thread_id, NULL, thread_function, thread_argument);
 *  pthread_join(thread_id, &return_value);
 *  pthread_detach(thread_id);
 */

/**
 * the (void *) type is a generic pointer type that can point to any data type.
 * This is useful in situations where you need to pass a pointer to a function,
 * but the type of the data being pointed to can vary.
 *
 * that why it's return type is also (void *).
 * and we typecast the type of arg to (int *) then derefrence the value.
 */

void *task(void *arg) {
  int id = *((int *)arg);
  printf("Thread id : %lu\n", pthread_self());  // return long unsigned int
  printf("Thread %d is running\n", id);
  sleep(2);
  printf("Thread %d is done\n", id);
  return NULL;
}

int main() {
  pthread_t thread_1, thread_2;
  int id1 = 1, id2 = 2;

  pthread_create(&thread_1, NULL, task, &id1);
  pthread_create(&thread_2, NULL, task, &id2);

  // If you not wait then process will exit before the its thread.
  pthread_join(thread_1, NULL);
  pthread_join(thread_2, NULL);

  printf("Main thread: all thread have completed\n");
  return 0;
}
