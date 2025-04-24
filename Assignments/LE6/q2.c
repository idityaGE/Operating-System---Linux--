#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int fprint_out(const char * filename, const int rows, const int cols, int matrix[][cols]) {
  FILE *fptr = NULL;
  fptr = fopen(filename, "w");
  if (fptr == NULL) {
    perror("Error opening file for writing");
    exit(1);
  }

  fprintf(fptr, "Output Matrix :\n");
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++)
      fprintf(fptr, "%d\t", matrix[i][j]);
    fprintf(fptr,"\n");
  }

  if (fclose(fptr) == EOF)
    perror("Error closing file after writing");
  else 
    printf("File closed successfully after writing.\n");
  
  return 0;
}

int read_matrix_from_file(FILE *inputFile, const int rows, const int cols, int matrix[][cols]) {
  char buffer[256];
  if (fgets(buffer, sizeof(buffer), inputFile) == NULL) {
    fprintf(stderr, "Error reading header line or unexpected end of file.\n");
    return -1;
  }

  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < cols; j++) {
      if (fscanf(inputFile, "%d", &matrix[i][j]) != 1) {
        fprintf(stderr, "Error reading integer at row %d, col %d\n", i, j);
        return -1;
      }
    }
    if (fgets(buffer, sizeof(buffer), inputFile) == NULL && i < rows - 1) {
      fprintf(stderr, "Error reading newline after row %d or unexpected end of file.\n", i);
    }
  }
  return 0;
}

typedef struct {
  int *row;
  int (*matrix2)[];
  int rows2;
  int cols2;
} thread_input;

void *multiply_one_row(void *arg) {
  thread_input *ti = (thread_input *)arg;
  int *res = (int *)malloc(ti->cols2 * sizeof(int));

  for(int i = 0; i < ti->cols2; i++)
    res[i] = 0;
  
  for(int i = 0; i < ti->cols2; i++)
    for (int j = 0; j < ti->rows2; j++)
      res[i] += (ti->row[j] * ti->matrix2[j][i]);
  return (void *)res;
}

int main() {
  FILE *input = NULL;
  input = fopen("input.txt", "r");
  if (input == NULL) {
    perror("Error opening file for reading input.txt");
    exit(1);
  }

  const int rows1 = 3;
  const int cols1 = 4;
  int matrix1[rows1][cols1];

  const int rows2 = 4;
  const int cols2 = 3;
  int matrix2[rows2][cols2];

  printf("Reading Matrix 1 from input.txt:\n");
  if (read_matrix_from_file(input, rows1, cols1, matrix1) != 0) {
    fprintf(stderr, "Failed to read Matrix 1.\n");
    fclose(input);
    exit(1);
  }


  printf("\nReading Matrix 2 from input.txt:\n");
  if (read_matrix_from_file(input, rows2, cols2, matrix2) != 0) {
    fprintf(stderr, "Failed to read Matrix 2.\n");
    fclose(input);
    exit(1);
  }
  
  int numOfThreads = rows1;
  pthread_t mulThread[numOfThreads];
  thread_input *ti_array = malloc(numOfThreads * sizeof(thread_input));

  for(int i = 0; i < numOfThreads; i++) {
    ti_array[i].row = matrix1[i];
    ti_array[i].matrix2 = (int (*)[])matrix2;
    ti_array[i].rows2 = rows2;
    ti_array[i].cols2 = cols2;
    
    if (pthread_create(&mulThread[i], NULL, multiply_one_row, &ti_array[i]) != 0) {
      perror("Failed to create thread");
      exit(1);
    }
  }

  int resultMatrix[rows1][cols2];
  
  for(int i = 0; i < numOfThreads; i++) {
    int *row_result;
    pthread_join(mulThread[i], (void**)&row_result);
    for(int j = 0; j < cols2; j++) 
      resultMatrix[i][j] = row_result[j];
    free(row_result);
  }

  if(fprint_out("output.txt", rows1, cols2, resultMatrix) != 0) {
    perror("Failed to write output in file");
    exit(1);
  }
  
  free(ti_array);

  if (fclose(input) == EOF) 
    perror("Error closing input file after reading");
  else 
    printf("\nInput file closed successfully.\n");
  
  return 0;
}
