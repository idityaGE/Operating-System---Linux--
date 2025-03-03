#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();
  int status;

  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    // Child process
    printf("Child process (PID: %d) running\n", getpid());
    sleep(2);  // Simulate work
    printf("Child process exiting\n");
    exit(42);  // Exit with a specific value
  } else {
    // Parent process
    printf("Parent waiting for child (PID: %d) to finish\n", pid);

    // Wait for the child to finish
    wait(&status);

    if (WIFEXITED(status)) {
      printf("Child exited with status: %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      printf("Child terminated by signal: %d\n", WTERMSIG(status));
    }
  }

  return 0;
}