#include <stdio.h>
#include <stdlib.h>

int *
randx(int n)
{
    int * vec = (int *)malloc(sizeof(int) * n);
    return(vec);
}

int main(int argc, char ** argv){

    int n = 1000000;
    int m = 1000;

    int * x = randx(n);
    int * y = randx(n);
    int * p = (int *)malloc(sizeof(int) * n);

    int ix = 0;
    for (int jx=0; jx < m; ++jx)
        for (int kx=0; kx < m; ++kx)
            p[jx + m*kx] = ix++;

    int a = 3;
    for (int kx=0; kx < n; ++kx) {
        int jx = p[kx];
        y[jx] += a * x[jx];
    }

    return(0);
}
