#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t parent_done = 0;

void handle_usr1(int sig) {
  parent_done = 1;
}

int main() {
  pid_t pid = fork();
  if (pid < 0) {
    perror("Fork Failed\n");
    exit(1);
  } else if (pid == 0) {
    signal(SIGUSR1, handle_usr1);

    while (!parent_done) {
      pause();
    }

    printf("Second\n");
    fflush(stdout);
  } else {
    // Allow child process time to set up signal handler
    // This delay is not related to the output order, 
    // but rather ensures proper signal handling setup
    usleep(10000); 

    printf("First\n");
    fflush(stdout);


    kill(pid, SIGUSR1);
    wait(NULL);
  }
  return 0;
}
