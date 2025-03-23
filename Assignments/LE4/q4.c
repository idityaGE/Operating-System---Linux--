#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

// Function to set process limits as a mitigation strategy
void set_process_limits() {
  struct rlimit limit;

  // Set maximum number of processes that can be created by this user
  limit.rlim_cur = 50;   // Soft limit
  limit.rlim_max = 100;  // Hard limit

  if (setrlimit(RLIMIT_NPROC, &limit) != 0) {
    perror("setrlimit failed");
    exit(1);
  }

  printf("Process creation limits set: soft=%lu, hard=%lu\n",
         limit.rlim_cur, limit.rlim_max);
}

// Fork bomb implementation (DANGEROUS - will crash your system if not mitigated)
void fork_bomb() {
  printf("Starting fork bomb - system will become unresponsive\n");
  printf("Ctrl+C to attempt termination\n");

  while (1) {
    pid_t pid = fork();

    if (pid < 0) {
      // If fork fails, log the error but continue
      perror("Fork failed");
      sleep(1);  // Slow down if we can't create more processes
    } else if (pid > 0) {
      // For simulation purposes, terminate parent occasionally
      // to prevent excessive process build-up during testing
      if (rand() % 10 == 0) {
        exit(0);
      }
    }
    // Both parent and child continue trying to fork
  }
}

int main(int argc, char *argv[]) {
  if (argc > 1 && strcmp(argv[1], "mitigate") == 0) {
    // Set process limits before running the fork bomb
    set_process_limits();
  }

  // CAUTION: Uncomment only in a safe environment like a VM
  // fork_bomb();

  return 0;
}
