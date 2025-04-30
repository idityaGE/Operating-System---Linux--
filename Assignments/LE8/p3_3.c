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

  printf("Sending SIGTERM to process %d\n", pid);
  if (kill(pid, SIGTERM) == 0) {
    printf("SIGTERM signal sent successfully\n");
  } else {
    perror("Failed to send SIGTERM");
    return 1;
  }

  // Wait for process to terminate
  sleep(1);

  printf("Checking if process %d is still running...\n", pid);
  char cmd[50];
  sprintf(cmd, "ps -p %d -o pid,cmd", pid);
  system(cmd);

  return 0;
}
