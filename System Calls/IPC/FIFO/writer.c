#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char *fifo = "/tmp/my_fifo";
  mkfifo(fifo, 0666);  // Create FIFO with read-write permission

  int fd = open(fifo, O_WRONLY);  // Open FIFO for writing
  char message[] = "Hello from FIFO!";
  write(fd, message, sizeof(message));  // Write message
  close(fd);                            // Close FIFO

  return 0;
}
