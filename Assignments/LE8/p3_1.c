#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_term(int sig) {
  printf("\n-----------\n");
  printf("SIGTERM received. Terminating...\n");
  printf("-----------\n");
  exit(0);
}

int main() {
  // Ignore SIGINT (Ctrl+C)
  signal(SIGINT, SIG_IGN);

  signal(SIGTERM, handle_term);

  printf("p1 process started. PID: %d\n", getpid());
  fflush(stdout);

  while (1) {
    sleep(10);
    printf("p1 is still running...\n");
    fflush(stdout);
  }

  return 0;
}
