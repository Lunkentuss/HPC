#include <stdio.h>
#include <stdlib.h>

#define SIZE 30000
#define TRUE 1
#define FALSE 1

struct run_flags
{
    unsigned int run_f1;
    unsigned int run_f2;
    unsigned int run_f3;
    unsigned int run_f4;
    unsigned int run_f5;
};

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

    printf("Array[0][0] = %d\n", as[0][0]);

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

    printf("as[0][0]: %d\n", as[0][0]);

    free(asentries);
    free(as);

    return;
}

struct matrix{
    int * asentries;
    int ** as;
    int m;
    int n;
};

struct matrix * make_matrix(int m, int n)
{
    struct matrix * mat = (struct matrix *)malloc(sizeof(struct matrix));
    int * asentries = (int *)malloc(sizeof(int) * m * n);
    int ** as = (int **)malloc(sizeof(int *) * m);

    for(int ix = 0, jx = 0 ; ix < m ; ix++, jx+=n)
        as[ix] = asentries + jx;

    mat->as = as;
    mat->asentries = asentries;
    mat->m = m;
    mat->n = n;

    return(mat);
};

void free_matrix(struct matrix * mat){
    free(mat->asentries);
    free(mat->as);

    return;
}

int main()
{
    printf("\n");
    printf("=====================================================\n");
    printf("=====================================================\n");
    printf("================== HPC ASSIGNMENT 1 =================\n");
    printf("=====================================================\n");
    printf("=====================================================\n");
    printf("\n");

    //allocate_array_stack();
    //allocate_array_heap();
    //fragmentation_avoid_false();
    //fragmentation_avoid_true();
    struct matrix * mat = make_matrix(5,10);
    for(int ix = 0 ; ix < mat->m ; ix++)
    for(int jx = 0 ; jx < mat->n ; jx++)
        mat->as[ix][jx] = ix * jx;

    printf("Last value: %d", mat->as[mat->m-1][mat->n-1]);

    printf("\n");
    return(0);
}
