#include <math.h>
#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIXSIZE 12000

////////// START BLOCK: DO NOT TOUCH ANY CODE IN THIS HELPER FUNCTIONS
bool checkIfFullMatrixIsCorrect(double** m) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    for (int j = 0; j < MATRIXSIZE; j++) {
      if (m[i][j] != m[j][i]) {
        return false;
      }

      int targetval = ((i / 2 + MATRIXSIZE) * (j / 2 - MATRIXSIZE)) % 25;
      if (i != j && i > j && m[i][j] != targetval) {
        return false;
      } else if (i == j && m[i][j] != i + 1) {
        return false;
      }
    }
  }
  return true;
}

bool checkIfSymmetricMatrixIsCorrect(double** m, double** m2) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    for (int j = 0; j < MATRIXSIZE; j++) {
      if (i >= j) {
        if (fabs(m[i][j] - m2[i][j]) > 0.00005) {
          printf("%f %f\n", m[i][j], m2[i][j]);
          return false;
        }
      } else {
        if (fabs(m[i][j] - m2[j][i]) > 0.00005) {
          printf("%f %f\n", m[i][j], m2[j][i]);
          return false;
        }
      }
    }
  }

  return true;
}

bool checkEquality(const double* v1, const double* v2) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    if (fabs((v1[i] - v2[i]) / v1[i]) > 0.005) {
      printf("Error at %i: %f vs. %f\n", i, v1[i], v2[i]);
      return false;
    }
  }

  printf("Correctness test passed.\n");
  return true;
}

void printMatrix(double** mat) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    for (int j = 0; j < MATRIXSIZE; j++) {
      printf("%f ", mat[i][j]);
    }
    printf("\n");
  }
}

void printVector(const double* vec) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    printf("%f ", vec[i]);
  }
  printf("\n");
}
////////// END BLOCK: DO NOT TOUCH ANY CODE IN THIS HELPER FUNCTIONS

void defaultMultiply(double* result, double** m, const double* v) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    result[i] = 0.0;
  }

  // TODO Implement b) and c) and d)

}

void symmetricMultiply(double* result, double** m, const double* v) {
  for (int i = 0; i < MATRIXSIZE; i++) {
    result[i] = 0.0;
  }

  // TODO implement f) and g)
}

int main(int argc, char* argv[]) {
  // Allocate a full matrix
  double** m = NULL;
  m          = (double**)malloc(MATRIXSIZE * sizeof(double*));
  for (int i = 0; i < MATRIXSIZE; i++) {
    m[i] = (double*)calloc(MATRIXSIZE, sizeof(double));
  }

  // TODO: Fill with Values a)

  if (!checkIfFullMatrixIsCorrect(m)) {
    printf("m is not a correct matrix.\n");
  } else {
    printf("m is a correct, symmetric matrix.\n");
  }

  // printMatrix(m);

  // Allocate a symmetric matrix
  double** m2 = NULL;

  // TODO implement e)

  if (!checkIfSymmetricMatrixIsCorrect(m, m2)) {
    printf("m2 is not correct.\n");
  } else {
    printf("m2 is a correct, symmetric matrix.\n");
  }

  double* v = (double*)malloc(MATRIXSIZE * sizeof(double));
  if (v == NULL) {
    printf("v allocation failed.\n");
    return EXIT_FAILURE;
  }

  double* r1 = (double*)malloc(MATRIXSIZE * sizeof(double));
  if (r1 == NULL) {
    printf("r1 allocation failed.\n");
    return EXIT_FAILURE;
  }

  double* r2 = (double*)malloc(MATRIXSIZE * sizeof(double));
  if (r2 == NULL) {
    printf("r2 allocation failed.\n");
    return EXIT_FAILURE;
  }

  for (int i = 0; i < MATRIXSIZE; i++) {
    v[i] = i % 3 + 1;
  }

  const double t1 = omp_get_wtime();
  defaultMultiply(r1, m, v);
  const double t2 = omp_get_wtime();
  printf("Reference %fs.\n", (t2 - t1));

  const double t3 = omp_get_wtime();
  symmetricMultiply(r2, m2, v);
  const double t4 = omp_get_wtime();
  printf("Symmetric %fs.\n", (t4 - t3));

  checkEquality(r1, r2);

  for (int i = 0; i < MATRIXSIZE; i++) {
    free(m[i]);
  }

  free(m);
  free(r1);
  free(r2);
  free(v);

  return 0;
}
