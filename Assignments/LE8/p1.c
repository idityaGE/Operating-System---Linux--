#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_intr(int sig) {
  printf("-----------\n");
  printf("SIGINT signal received\n");
  printf("-----------\n");
  exit(0);
}

int main() {
  signal(SIGINT, handle_intr);
  // printf("PID : %d\n", getpid());
  // fflush(stdout); // displayed immediately

  for (;;) pause();
  return 0;
}
