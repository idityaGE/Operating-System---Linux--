#include <stdio.h>
#include <unistd.h>



int main() {
  int fd[2];  // File descriptors: fd[0] for read, fd[1] for write

  if (pipe(fd) == -1) {  // Returns 0 on success, -1 on failure.
    perror("pipe failed");
    return 1;
  }

  printf("Pipe created: Read FD = %d, Write FD = %d\n", fd[0], fd[1]);

  return 0;
}


