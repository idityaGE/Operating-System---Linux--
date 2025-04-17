#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 




int main() {
  FILE *input = NULL;
  input = fopen("input.txt", "r");
  if (input == NULL) {
    perror("Error opening file for reading");
    exit(1);
  }

  const int rows = 3;
  const int cols = 4;
  int matrix[rows][cols];

  printf("Reading matrix elements from input.txt:\n");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (fscanf(input, "%d", &matrix[i][j]) != 1) {
        fprintf(stderr, "Error reading integer from file at row %d, col %d\n", i, j);
        fclose(input);
        exit(1);
      }
    }
  }

  printf("\nMatrix read from file:\n");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }


  if (fclose(input) == EOF) {
    perror("Error closing file after reading");
  } else {
    printf("\nFile closed successfully.\n");
  }

  FILE *output = NULL;
  output = fopen("output.txt", "w");
  if (output == NULL) {
    perror("Error opening file for writing");
    exit(1);
  }

  fprintf(output, "Output Matrix :\n");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) 
      fprintf(output, "%d\t", matrix[i][j]);
    fprintf(output,"\n");
  }

  if (fclose(output) == EOF) {
    perror("Error closing file after writing");
  } else {
    printf("File closed successfully after writing.\n");
  }

  return 0;
}
