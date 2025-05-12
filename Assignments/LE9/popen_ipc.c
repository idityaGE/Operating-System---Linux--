#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  char buffer[100];

  for (int i = 0; i < 10; i++) {
    char msg[100];
    sprintf(msg, "echo \"Message : %d\"", i + 1);

    FILE *fp = popen(msg, "r");
    // sleep(1);
    if (fp == NULL) {
      perror("popen failed");
      exit(1);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
      printf("Consumer received: %s", buffer);
    }

    pclose(fp);
  }

  return 0;
}
