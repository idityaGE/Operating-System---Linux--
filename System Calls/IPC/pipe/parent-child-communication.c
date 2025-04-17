#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * When fork() is called, the child inherits copies of 
 * all the parent's file descriptors, so both processes 
 * initially have access to both ends of the pipe. 
 * Proper closing of unused ends ensures clean communication.
 */

int main() {
  /**
   * fd is an array of two integers that stores file descriptors
   * created by the pipe() system call:
   * - fd[0] becomes the read end of the pipe
   * - fd[1] becomes the write end of the pipe
   */

  int fd[2];
  pid_t pid;
  char buffer[100];

  if (pipe(fd) == -1) {
    perror("pipe failed");
    return 1;
  }

  pid = fork();

  if (pid < 0) {
    perror("fork failed");
    return 1;
  }

  /**
   * The close() function closes a file descriptor,
   * telling the operating system you're done with that particular I/O channel:
   * - close(fd[0]); // Closes the read end of the pipe
   * - close(fd[1]); // Closes the write end of the pipe
   */

  if (pid == 0) {  // child process
    close(fd[1]);  // close write end
    read(fd[0], buffer, sizeof(buffer));
    printf("Child received: %s\n", buffer);
    close(fd[0]);
  } else {
    close(fd[0]);  // parent close read end
    char msg[] = "Hello from Parent !";
    write(fd[1], msg, strlen(msg) + 1);
    close(fd[1]);
  }

  return 0;
}

/**
 * Why Closing Unused Ends Is Important
 * 1. Resource Management: File descriptors are limited system resources
 * 2. Proper EOF Handling: If all write ends are closed, 
 *    a read will return 0 (EOF) when all data is consumed
 * 3. Preventing Deadlocks: A program can hang if it's waiting for data 
 *    from a pipe that will never receive more data
 */

// Parent Process                      Child Process
// +---------------+                 +---------------+
// |               |                 |               |
// | writes data   |                 | reads data    |
// | to fd[1]      |                 | from fd[0]    |
// |               |                 |               |
// +-------+-------+                 +-------+-------+
//         |                                 ^
//         |                                 |
//         |        +---------------+        |
//         +------->|  Kernel Pipe  |--------+
//                  |  Buffer       |
//                  +---------------+
