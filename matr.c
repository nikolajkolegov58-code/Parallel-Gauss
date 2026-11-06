#include "matr.h"
#include <math.h>

int max(int, int);

int max(int x, int y) {if (x>y) return x; else return y;}

double f(int k, int n, int i, int j) {
int x;

if (k==1) {x=n-max(i, j); return (double)x;}

if (k==2) {x=1+max(i, j); return (double)x;}

if (k==3) return fabs((double)(i-j));

if (k==4) {x=i+j+1; return 1.0/((double)x);}

return 0.0;
}
