#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE (int)pow(2,20)

void
vec(int * ints, const int n)
{
    for(int i = 0 ; i < n ; i++) {
        ints[i] = i + 1;
    }
}

int
main(int argc, char ** argv) {
    int * ints = (int *)malloc(sizeof(int) * SIZE);
    vec(ints, SIZE);
    FILE * file = fopen("test.dat", "w");
    fwrite(ints, sizeof(int), SIZE, file);
    return(0);
}
