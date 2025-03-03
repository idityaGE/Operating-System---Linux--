#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Program A started (PID: %d)\n", getpid());

  // Check if we were called recursively
  if (argc > 1 && strcmp(argv[1], "recursive") == 0) {
    printf("This is a recursive execution of Program A, stopping here\n");
    return 0;  // Stop the recursion
  }

  pid_t pid = fork();

  if (pid < 0) {
    perror("Fork failed\n");
    exit(1);
  } else if (pid == 0) {
    printf("Child B created (PID: %d)\n", getpid());

    printf("Child B replacing itself with Program C...\n");
    execl("./program_c", "program_c", NULL);

    perror("execl failed to run program_c");
    exit(1);
  } else {
    printf("Parent A waiting for child to finish\n");
    wait(NULL);

    printf("Parent A replacing itself with a new instance of Program A...\n");
    execl("./q3", "q3", "recursive", NULL);

    // This code will only run if execl fails
    perror("execl failed to re-run program_a");
    exit(1);
  }

  return 0;
}