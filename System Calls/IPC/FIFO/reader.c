#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char *fifo = "/tmp/my_fifo";
  mkfifo(fifo, 0666);  // Create FIFO

  int fd = open(fifo, O_RDONLY);  // Open FIFO for reading
  char buffer[100];
  read(fd, buffer, sizeof(buffer));  // Read message
  printf("Received: %s\n", buffer);
  close(fd);  // Close FIFO

  return 0;
}
