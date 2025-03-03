#include <stdio.h>
#include <unistd.h>

int main() {
  pid_t pid;

  // Create a new process
  pid = fork();

  if (pid < 0) {
    // Error occurred
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    // Child process
    printf("Child process: My PID is %d, my parent's PID is %d\n",
           getpid(), getppid());
  } else {
    // Parent process
    printf("Parent process: My PID is %d, my child's PID is %d\n",
           getpid(), pid);
  }

  return 0;
}