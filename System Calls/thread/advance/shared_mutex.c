#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
  pthread_mutex_t mutex;
  int counter;
} shared_data_t;

int main() {
  // Create shared memory region for mutex
  shared_data_t *shared_data = mmap(NULL, sizeof(shared_data_t),
                                    PROT_READ | PROT_WRITE,
                                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  if (shared_data == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  // Initialize mutex with process-shared attribute
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(&shared_data->mutex, &attr);
  pthread_mutexattr_destroy(&attr);

  shared_data->counter = 0;

  // Create a child process
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    // Child process
    printf("Child process: Attempting to lock mutex\n");
    pthread_mutex_lock(&shared_data->mutex);
    printf("Child process: Got the lock\n");

    shared_data->counter += 10;
    printf("Child process: Incremented counter to %d\n", shared_data->counter);
    sleep(2);

    printf("Child process: Unlocking mutex\n");
    pthread_mutex_unlock(&shared_data->mutex);
    exit(0);
  } else {
    // Parent process
    sleep(1);  // Give child time to acquire the lock first

    printf("Parent process: Attempting to lock mutex\n");
    pthread_mutex_lock(&shared_data->mutex);
    printf("Parent process: Got the lock\n");

    shared_data->counter += 5;
    printf("Parent process: Incremented counter to %d\n", shared_data->counter);

    printf("Parent process: Unlocking mutex\n");
    pthread_mutex_unlock(&shared_data->mutex);

    // Wait for child to finish
    wait(NULL);

    // Clean up
    pthread_mutex_destroy(&shared_data->mutex);
    munmap(shared_data, sizeof(shared_data_t));

    printf("Final counter value: %d\n", shared_data->counter);
  }

  return 0;
}
