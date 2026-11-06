#include "gauss.h"
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

int g_n, g_j; double** g_A; double** g_B;            //new global variables
int g_lo[256], g_hi[256];

void* direct(void* arg) {                         //thread function for direct moving
int t = (int)(intptr_t)arg;
int i, k, j = g_j, n = g_n; double bf;
double** A = g_A;
double** B = g_B;

for (i = g_lo[t]; i < g_hi[t]; i++) {
bf = A[i][j];
for (k = j; k < n; k++) A[i][k] -= bf * A[j][k];
for (k = 0; k < n; k++) B[i][k] -= bf * B[j][k];
A[i][j] = 0.0;
}
return NULL;
}

void* inverse(void* arg) {                          //thread function for inverse moving
int t = (int)(intptr_t)arg;
int i, k, j = g_j, n = g_n; double bf;
double** A = g_A;
double** B = g_B;

for (i = g_lo[t]; i < g_hi[t]; i++) {
bf = A[i][j];
for (k = 0; k < n; k++) B[i][k] -= bf * B[j][k];
A[i][j] = 0.0;
}
return NULL;
}

void range(int lo, int hi, int p, int t, int* outLo, int* outHi) {       //working range for thread t
int len = hi - lo; int base, rem, s;
if (len <= 0) {*outLo = 0; *outHi = 0; return;}
base = len / p, rem = len % p;
s = lo + t * base + (t < rem ? t : rem);
*outLo = s;
*outHi = s + base + (t < rem ? 1 : 0);
}

int Gauss(int n, double** A, double** B, int threads) {
int i, j, i0=0, k, lo, hi, t; double M=0, eps=1e-8, bufer;
pthread_t* tids;

if (threads < 1) threads = 1;
if (threads > n) threads = n;
if (threads > 256) threads = 256;
int p = threads;

for (i = 0; i < n; i++) { for (j = 0; j < n; j++) if (fabs(A[i][j]) > M) M = fabs(A[i][j]); }
eps *= M; M = 0;                         //choose eps

g_n = n;
g_A = A;
g_B = B;

tids = (pthread_t*)malloc(sizeof(pthread_t) * p);
if (!tids) return 0;

for (j = 0; j < n; j++) {                  //direct moving
M = 0;
for (i = j; i < n; i++) { if (fabs(A[i][j]) > fabs(M)) {i0 = i; M = A[i][j];} }      //sequantial max element
if (fabs(M) < eps) {free(tids); return 0;}

for (k = j; k < n; k++) { bufer = A[j][k]; A[j][k] = A[i0][k]; A[i0][k] = bufer; }      //sequential swap strings
for (k = 0; k < n; k++) { bufer = B[j][k]; B[j][k] = B[i0][k]; B[i0][k] = bufer; }

for (k = j; k < n; k++) A[j][k] /= M;
for (k = 0; k < n; k++) B[j][k] /= M;         //sequential normalization

g_j = j;

for (t = 0; t < p; t++) {                  //parallel calculations
range(j + 1, n, p, t, &lo, &hi);
g_lo[t] = lo;
g_hi[t] = hi;
pthread_create(&tids[t], NULL, direct, (void*)(intptr_t)t);
}
for (t = 0; t < p; t++) pthread_join(tids[t], NULL);
}


for (j = n-1; j > 0; j--) {            //inverse moving
g_j = j;
for (t = 0; t < p; t++) {
range(0, j, p, t, &lo, &hi);
g_lo[t] = lo;
g_hi[t] = hi;
pthread_create(&tids[t], NULL, inverse, (void*)(intptr_t)t);       //parallel calculations
}
for (t = 0; t < p; t++) pthread_join(tids[t], NULL);
}

free(tids);
return 1;
}



















