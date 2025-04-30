#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork failed");
    exit(1);
  } else if (pid == 0) {
    printf("Starting pause program\n");
    execl("./p1", "p1", NULL);
    perror("execl failed to run p1");
    exit(1);
  } else {
    sleep(2);
    printf("Sending SIGINT to child process (PID: %d)\n", pid);
    if (kill(pid, SIGINT) != 0) {
      perror("Failed to send signal");
      exit(1);
    }
    wait(NULL);
    printf("Child process terminated\n");
  }

  return 0;
}
