#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    // Child process runs the ls command
    printf("Child is about to execute ls -l\n");

    // Replace the child process with the ls command
    execl("/bin/ls", "ls", "-l", NULL);

    // If execl returns, an error occurred
    perror("execl failed");
    exit(1);
  } else {
    // Parent process
    printf("Parent process continues execution\n");
  }

  return 0;
}