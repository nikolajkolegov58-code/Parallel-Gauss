#include "norma.h"
#include <math.h>

double norm(int n, double **A, double **B) {
double S, N = 0; int i, j, k;

for (i = 0; i < n; i++) for (j = 0; j < n; j++) {
S = 0; for (k = 0; k < n; k++) S += A[i][k] * B[k][j];
S -= (double)(i == j); N += fabs(S);
}

N *= 1e8;

return N;
}