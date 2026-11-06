#include <stdio.h>
#include <stdlib.h>
#include "matr.h"
#include "gauss.h"
#include "norma.h"
#include <time.h>

int main(int argc, char** argv) {
int n, m, k, i, j, threads; char* filename; double **A, **B; FILE *fin; clock_t start, end;
n = atoi(argv[1]); m = atoi(argv[2]); k = atoi(argv[3]); threads = atoi(argv[4]);
if (argc == 6) { filename = argv[5]; fin = fopen(filename, "r"); if (fin == NULL) return -1; }

A = (double **)malloc(n * sizeof(double *));
B = (double **)malloc(n * sizeof(double *));
for (i = 0; i < n; i++) { A[i] = (double *)malloc(n * sizeof(double)); B[i] = (double *)malloc(n * sizeof(double)); }

if (!k) for (i = 0; i < n; i++) for (j = 0; j < n; j++) { fscanf(fin, "%lf", &A[i][j]); B[i][j] = (double)(i == j); }
else for (i = 0; i < n; i++) for (j = 0; j < n; j++) { A[i][j] = f(k, n, i, j); B[i][j] = (double)(i == j); }

start = clock();
i = Gauss(n, A, B, threads);
end = clock();

if (!i) {
printf("error\n");
for (i = 0; i < n; i++) { free(A[i]); free(B[i]); }
free(A);
free(B);

return -1;
}

for (i = 0; i < m; i++) { for (j = 0; j < m; j++) printf("%lf ", B[i][j]); printf("\n"); }

if (!k) { rewind(fin); for (i = 0; i < n; i++) { for (j = 0; j < n; j++) fscanf(fin, "%lf", &A[i][j]); } fclose(fin); }
else for (i=0; i<n; i++) for (j=0; j<n; j++) A[i][j]=f(k, n, i, j);

printf("Time = %lf sec\n", ((double)(end-start)) / CLOCKS_PER_SEC);

printf("Residual norm = %lf\n", norm(n, A, B));

for (i = 0; i < n; i++) { free(A[i]); free(B[i]); }
free(A);
free(B);

return 0;
}














