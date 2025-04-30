#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <pid>\n", argv[0]);
    return 1;
  }

  pid_t pid = atoi(argv[1]);

  printf("Sending SIGINT to process %d\n", pid);
  if (kill(pid, SIGINT) == 0) {
    printf("SIGINT signal sent\n");
  } else {
    perror("Failed to send signal SIGINT");
    exit(1);
  }

  printf("Checking if process %d is still running...\n", pid);
  char cmd[50];
  sprintf(cmd, "ps -p %d -o pid,cmd", pid);
  system(cmd);

  return 0;
}
