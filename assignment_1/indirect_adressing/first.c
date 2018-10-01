#include <stdio.h>
#include <stdlib.h>

int *
ones(int n)
{
    int * vec = (int *)malloc(sizeof(int) * n);
    return(vec);
}

int main(int argc, char ** argv){

    int n = 1000000;
    int m = 1000;

    int * x = ones(n);
    int * y = ones(n);
    int * p = ones(n);
    int a = 3;

    for (int kx=0; kx < n; ++kx) {
        int jx = p[kx];
        y[jx] += a * x[jx];
    }

    int ix = 0;
    for (int jx=0; jx < m; ++jx)
        for (int kx=0; kx < m; ++kx)
            p[jx + m*kx] = ix++;

    return(0);
}
