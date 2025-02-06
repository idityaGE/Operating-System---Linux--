#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main() {
  pid_t pid;
  pid = fork();

  if (pid < 0) {
    printf("Fork failed\n");
    exit(1);
  } else if (pid == 0) {
    printf("Child process\n");
    printf("Child process ID: %d\n", getpid());
    printf("Parent process ID: %d\n", getppid());
  } else {
    printf("Parent process\n");
    printf("Parent process ID: %d\n", getpid());
    printf("Child process ID: %d\n", pid);
    exit(0);
  }
}