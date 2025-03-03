#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void child_first();
void parent_first();

/**
 * An orphan process is a process whose parent has terminated, 
 * while the child is still running. 
 * The orphan is adopted by the init process (PID 1).
 */

int main() {
  printf("---- Child First Execution ----\n");
  child_first();

  printf("\n---- Parent First Execution ----\n");
  parent_first();
  return 0;
}

void child_first() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork failed");
    exit(1);
  } else if (pid == 0) {
    printf("Child process executing first (PID: %d)\n", getpid());
    sleep(2);
    printf("Child process completed.\n");
  } else {
    wait(NULL);
    printf("Parent process executing after child (PID: %d)\n", getpid());
  }
}

// Orphan Process
void parent_first() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork failed");
    exit(1);
  } else if (pid == 0) {
    printf("Child (PID: %d) running, parent is (PID: %d)\n", getpid(), getppid());
    sleep(3);  // Give parent time to exit
    printf("Child's new parent is PID: %d (init/systemd)\n", getppid());
    sleep(5);
    printf("Child process executing after parent (PID: %d)\n", getpid());
    printf("Child exiting\n");
  } else {
    printf("Parent process executing first (PID: %d)\n", getpid());
    printf("Parent exiting, child will become orphan\n");
    exit(0);
  }
}