#include <stdio.h>

int main() {
  FILE *fp;

  char buffer[100];

  fp = popen("ls -l", "r");
  if (fp == NULL) {
    perror("popen Failed");
    return 1;
  }

  while(fgets(buffer, sizeof(buffer), fp) != NULL) {
    printf("%s\n", buffer);
  }

  pclose(fp);
  
  return 0;
}
