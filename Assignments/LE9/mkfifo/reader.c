#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  int fd;
  char *fifo = "/tmp/myfifo";
  char buffer[100];

  fd = open(fifo, O_RDONLY);
  read(fd, buffer, sizeof(buffer));
  close(fd);

  printf("Output : %s\n", buffer);
  return 0;
}
