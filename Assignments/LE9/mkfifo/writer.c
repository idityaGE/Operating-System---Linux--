#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  int fd;
  char *fifo = "/tmp/myfifo";
  char *msg = "Hello from Adii";

  mkfifo(fifo, 0666);

  fd = open(fifo, O_WRONLY);
  write(fd, msg, sizeof(msg));
  close(fd);

  return 0;
}
