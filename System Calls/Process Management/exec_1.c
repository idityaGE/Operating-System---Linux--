#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

void main() {
  printf("Ecexuting `ls`\n");
  execl("/bin/ls", "ls", "-la", NULL);

  perror("execl() failed to run `ls`");
  exit(1);
}