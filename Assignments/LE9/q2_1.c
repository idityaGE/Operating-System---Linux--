#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid;
  int fd[2];
  char buffer[100];

  if (pipe(fd) == -1) {
    perror("pipe failed");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid < 0) {
    perror("fork Failed");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    // Child
    close(fd[1]);
    printf("Child start consuming\n");
    for (int i = 0; i < 10; i++) {
      // If the pipe is empty but still open for writing (parent hasn't closed fd[1]), the read() blocks — it waits for the parent to write something.
      read(fd[0], buffer, 100); 
      printf("Child received: %s\n", buffer);
    }
    close(fd[0]);
  } else {
    // Parent
    close(fd[0]);
    printf("Parent start producing\n");
    for (int i = 0; i < 10; i++) {
      char message[100];
      snprintf(message, sizeof(message), "Message no. : %d", i + 1);
      write(fd[1], message, 100); 
      sleep(1);
    }
    close(fd[1]);
    waitpid(pid, NULL, 0);
  }

  return 0;
}
