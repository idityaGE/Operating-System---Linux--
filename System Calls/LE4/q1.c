#include <stdio.h>
// #include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void child_parent();
void parent_child();

int main() {
  // child_parent();
  parent_child();
  return 0;
}

void child_parent() {
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

void parent_child() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork failed");
    exit(1);
  } else if (pid == 0) {
    sleep(2);
    printf("Child process executing after parent (PID: %d)\n", getpid());
  } else {
    printf("Parent process executing first (PID: %d)\n", getpid());
  }
}