#include <stdlib.h>

int main() {
  int *array1 = malloc(sizeof(int) * 2);
  int array2[2];
  array1[0] = 0;
  array2[0] = 0;
  array1[1] = 1;
  array2[1] = 1;

  int a = 0;
  int *b = &a;// b is pointer to a
  int *c = array1;
  int *d = &(array1[0]);

  d = b;//d is pointer to a
  *d = 123; // a = 123
  a = 42;
  array1[0] = 42;
  a = *c + *b;
  d = 54321;

  a = *(c + array2[0]);
  free(array1);
  free(array2);
  array2 = 321;
  array1[0] = a;
}
