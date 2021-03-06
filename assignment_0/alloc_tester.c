#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "matrix.h"

#define SIZE 50000

void allocate_array_stack()
{
    int as[SIZE];
    for(size_t ix=0 ; ix < SIZE ; ix++)
        as[ix] = 0;

    printf("First array number: %d\n", as[0]);
    return;
}

void allocate_array_heap()
{
    int * as = (int *)malloc(sizeof(int) * SIZE);
    for(size_t ix=0 ; ix < SIZE ; ix++)
        as[ix] = 0;

    printf("First array number: %d\n", as[0]);
    free(as);
    return;
}

void fragmentation_avoid_false()
{
    int ** as = (int **)malloc(sizeof(int *) * SIZE);
    for(size_t ix = 0 ; ix < SIZE ; ix++)
        as[ix] = (int *)malloc(sizeof(int) * SIZE);

    for(size_t ix = 0 ; ix < SIZE ; ix++)
        for(size_t jx = 0 ; jx < SIZE ; jx++)
            as[ix][jx] = 0;

    printf("Matrix[0][0]: %d\n", as[0][0]);

    for(size_t ix = 0 ; ix < SIZE ; ix++)
        free(as[ix]);

    return;
}

void fragmentation_avoid_true()
{
    int * asentries = (int *)malloc(sizeof(int) * SIZE * SIZE);
    int ** as = (int **)malloc(sizeof(int *) * SIZE);

    for(int ix = 0, jx = 0 ; ix < SIZE ; ix++, jx += SIZE)
        as[ix] = asentries + jx;

    for(int ix = 0 ; ix < SIZE ; ix++)
        for(int jx = 0 ; jx < SIZE ; jx++)
            as[ix][jx] = 0;

    printf("Matrix[0][0]: %d\n", as[0][0]);

    free(asentries);
    free(as);

    return;
}

int main(int argc, char * argv[]) {
    allocate_array_stack();
    allocate_array_heap();
    fragmentation_avoid_false();
    fragmentation_avoid_true();
    struct matrix * mat = matrix_init(5,10);
    for(int ix = 0 ; ix < mat->m ; ix++)
        for(int jx = 0 ; jx < mat->n ; jx++)
            mat->as[ix][jx] = ix * jx;

    printf("\n");
    return(0);
}
