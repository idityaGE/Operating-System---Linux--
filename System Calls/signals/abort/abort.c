#include <stdio.h>
#include <stdlib.h>

int main() {
  int i = 0;
  printf("Before the abort");
  abort();
  printf("After the abort");
  return 0;
}
