#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  int fd;
  char *fifo = "/tmp/myfifo";
  char message[100];

  mkfifo(fifo, 0666);
  fd = open(fifo, O_WRONLY);

  for (int i = 0; i < 10; i++) {
    snprintf(message, sizeof(message), "Message : %d", (i + 1));
    write(fd, message, 100);
    sleep(1);
  }

  close(fd);

  return 0;
}
