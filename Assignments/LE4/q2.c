#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * A zombie process is a process that has completed execution but still has an entry in
 * the process table because its parent hasn't called wait() to read its exit status.
 */

int main() {
  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork Failed\n");
    return 1;
  } else if (pid == 0) {
    printf("Child (PID: %d) running\n", getpid());
    printf("Child exiting\n");
    exit(0);
  } else {
    printf("Parent (PID: %d) created child (PID: %d)\n", getpid(), pid);
    printf("Parent sleeping for 10 seconds - child becomes zombie\n");
    printf("Run 'ps -aux | grep Z' in another terminal to see the zombie\n");
    sleep(15);

    int signal;
    waitpid(pid, &signal, 0);
    printf("Signal from chiled is %d\n", signal);
    printf("Parent collected child's exit status, zombie is gone\n");
  }

  return 0;
}
