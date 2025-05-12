#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int fd[2];
  // fd[0] -> reader
  // fd[1] -> writer

  pid_t pid;
  char buffer[100];

  pipe(fd);

  pid = fork();
  if (pid < 0) {
    perror("Fork Failed");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    close(fd[1]);  // writer end
    read(fd[0], buffer, sizeof(buffer));
    close(fd[0]);
    printf("Child received: %s\n", buffer);
  } else {
    close(fd[0]);  // reader end
    char message[] = "Hello from parent";
    write(fd[1], message, strlen(message) + 1);
    close(fd[1]);
    waitpid(pid, NULL, 0);
  }

  return 0;
}
