#ifndef MATRIX_H
# define MATRIX_H

#include <stdbool.h>

#define MATRIX_TYPE_INT int
#define MATRIX_TYPE_DOUBLE double

#ifndef MATRIX_TYPE 
    #define MATRIX_TYPE int
    #define MATRIX_TYPE_STR "d"

#elif MATRIX_TYPE == MATRIX_TYPE_INT
    #define MATRIX_TYPE_STR "d"

#elif MATRIX_TYPE == MATRIX_TYPE_INT
    #define MATRIX_TYPE_STR "lf"

#endif //MATRIX_TYPE

struct matrix{
    MATRIX_TYPE * asentries;
    MATRIX_TYPE ** as;
    int m;
    int n;
};

struct matrix * matrix_init(int m, int n)
{
    struct matrix * mat = (struct matrix *)malloc(sizeof(struct matrix));
    MATRIX_TYPE * asentries = (MATRIX_TYPE *)malloc(sizeof(MATRIX_TYPE) * m * n);
    MATRIX_TYPE ** as = (MATRIX_TYPE **)malloc(sizeof(MATRIX_TYPE *) * m);

    for(int ix = 0, jx = 0 ; ix < m ; ix++, jx+=n)
        as[ix] = asentries + jx;

    mat->as = as;
    mat->asentries = asentries;
    mat->m = m;
    mat->n = n;

    return(mat);
};

bool matrix_equal(struct matrix * mat1, struct matrix * mat2) {
    if (mat1->m != mat2->m) {
        return(false);
    }

    if (mat1->n != mat2->n) {
        return(false);
    }

    for (int i = 0 ; i < mat1->m ; i++) {
        for (int j = 0 ; j < mat2->n ; j++) {
            if (mat1->as[i][j] != mat2->as[i][j]){
                return(false);
            }
        }
    }

    return(true);
}

void matrix_print(struct matrix * mat) {
    for (int i = 0 ; i < mat->m ; i++) {
        for (int j = 0 ; j < mat->n ; j++) {
            printf("%" MATRIX_TYPE_STR " ", mat->as[i][j]);
        }
        printf("\n");
    }
    return;
}

void matrix_free(struct matrix * mat){
    free(mat->asentries);
    free(mat->as);

    return;
}

void write_matrix(FILE * fp, struct matrix * mat)
{
    fwrite(&mat->m, sizeof(int), 1, fp);
    fwrite(&mat->n, sizeof(int), 1, fp);

    fwrite(*mat->as, sizeof(int), mat->m * mat->n, fp);
    return;
}

struct matrix * read_matrix(FILE * fp)
{
    int m, n;

    fread(&m, sizeof(int), 1, fp);
    fread(&n, sizeof(int), 1, fp);

    struct matrix * mat = matrix_init(m, n);
    fread(*mat->as, sizeof(int), m * n, fp);

    return(mat);
}

#endif /* MATRIX_H */
