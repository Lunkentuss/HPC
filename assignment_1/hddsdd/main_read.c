#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE (int)pow(2,20)

int
main(int argc, char ** argv) {
    int * ints = (int *)malloc(sizeof(int) * SIZE);
    FILE * file = fopen("test.dat", "r");
    fread(ints, sizeof(int), SIZE, file);
    return(0);
}
