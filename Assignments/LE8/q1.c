#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void handle_fpe(int sig) {
  printf("Do not divide by zero\n");
  exit(1);
}

int main() {
  signal(SIGFPE, handle_fpe);
  int numerator = 5, denominator = 0;
  printf("%d", numerator / denominator);
  return 0;
}
