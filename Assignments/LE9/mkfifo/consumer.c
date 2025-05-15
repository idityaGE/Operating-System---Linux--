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

  for (int i = 0; i < 100; i++) {
    read(fd, buffer, sizeof(buffer));
    printf("Output : %s\n", buffer);
  }

  close(fd);

  return 0;
}
