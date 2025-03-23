#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
int ready = 0;

/**
 * waiter thread waits for signaler to set ready = 1.
 * pthread_cond_wait() makes the thread wait until the condition is met.
 * pthread_cond_signal() wakes up the waiting thread.
 */

/**
 * Here one question arise in my mind :
 * Question :
 * Here if the waiter function lock the mutex then
 * It will not allow the signaler function to lock mutex and
 * it will not be able to set ready = 1 and
 * not be able to send the con_signer ?
 *
 * Answer :
 * Why Does the waiter Not Block the signaler?
 * The key behavior here is how pthread_cond_wait(&cond, &mutex) works.
 * When a thread calls pthread_cond_wait(&cond, &mutex), it atomically:
 * - Releases the mutex, allowing other threads to acquire it.
 * - Puts the calling thread (waiter) into a waiting state.
 * - When another thread signals the condition (pthread_cond_signal(&cond)),
 *    the waiting thread is woken up.
 * - Before continuing, pthread_cond_wait() reacquires the mutex to ensure safe access.
 *
 * -- This means the waiter temporarily releases the lock while waiting for a signal
 * allowing the signaler to lock the mutex, modify ready = 1, and signal the condition.
 */

void *waiter(void *arg) {
  pthread_mutex_lock(&mutex);
  while (!ready) {
    pthread_cond_wait(&cond, &mutex);
  }
  printf("Thread proceeding...\n");
  pthread_mutex_unlock(&mutex);
  return NULL;
}

void *signaler(void *arg) {
  pthread_mutex_lock(&mutex);
  ready = 1;
  pthread_cond_signal(&cond);
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main() {
  pthread_t t1, t2;

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  pthread_create(&t1, NULL, waiter, NULL);
  pthread_create(&t2, NULL, signaler, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}
