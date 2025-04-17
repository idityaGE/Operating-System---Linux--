#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  double a, b, c;
} quadatic_equation;

typedef struct {
  double root1, root2;
} discreminent;

void *calculate_discriminent(void *arg) {
  quadatic_equation *equa = (quadatic_equation *)arg;
  discreminent *result = malloc(sizeof(discreminent));
  
  double discriminant = pow(equa->b, 2) - 4 * equa->a * equa->c;
  result->root1 = (-1 * equa->b + sqrt(discriminant)) / (2 * equa->a);
  result->root2 = (-1 * equa->b - sqrt(discriminant)) / (2 * equa->a);
  
  return (void *)result;
}

long long factorial(int n) {
  if (n <= 1) return 1;
  return (n * factorial(n - 1));
}

typedef struct {
  int x, n;
} expo;

double calculate_e_series(int x, int n) {
  double sum = 0.0;
  for (int i = 0; i <= n; i++)
    sum += pow(x, i) / (double)factorial(i);
  return sum;
}

void *calculate_ex(void *arg) {
  expo *xn = (expo *)arg;
  double *result = malloc(sizeof(double));
  *result = calculate_e_series(xn->x, xn->n);
  return (void *)result;
}

int main() {
  quadatic_equation equa = {
      .a = 1,
      .b = 3,
      .c = -4};

  expo xn = {
      .n = 5,
      .x = 2};

  pthread_t t1, t2;
  discreminent *roots;
  double *e_approx;

  pthread_create(&t1, NULL, calculate_discriminent, &equa);
  pthread_create(&t2, NULL, calculate_ex, &xn);

  pthread_join(t1, (void **)&roots);
  pthread_join(t2, (void **)&e_approx);

  printf("Quadratic equation: %.1fx² + %.1fx + %.1f\n", equa.a, equa.b, equa.c);
  printf("Roots: %.4f and %.4f\n\n", roots->root1, roots->root2);

  printf("e^%d approximation (up to %d terms): %.6f\n", xn.x, xn.n, *e_approx);

  free(roots);
  free(e_approx);

  return 0;
}
