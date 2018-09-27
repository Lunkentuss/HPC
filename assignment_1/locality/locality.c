#include <stdio.h>
#include <stdlib.h>

#define MATRIX_TYPE double
#include <matrix.h>

#define SIZE 10

/* Prints a vector of type MATRIX_TYPE */
void vec_print(MATRIX_TYPE * vec, int count) {
    for (int i = 0 ; i < count ; i++) {
        printf("%lf ", vec[i]);
    }
    printf("\n");
}

/* Insert any matrix values */
void
mult_table(struct matrix * mat) {
    for(int i = 0 ; i < mat->m ; i++){
        for(int j = 1 ; j < mat->n ; j++){
            mat->as[i][j] = (MATRIX_TYPE)i * j;
        }
    }
}

/* Calculates row sum of a matrix*/
void
row_sums(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
)
{
  for ( size_t ix=0; ix < nrs; ++ix ) {
    double sum = 0;
    for ( size_t jx=0; jx < ncs; ++jx )
      sum += matrix[ix][jx];
    sums[ix] = sum;
  }
}

/* Calculates column sum of a matrix*/
void
col_sums(
  double * sums,
  const double ** matrix,
  size_t nrs,
  size_t ncs
  )
{
  for ( size_t jx=0; jx < ncs; ++jx ) {
    double sum = 0;
    for ( size_t ix=0; ix < nrs; ++ix )
      sum += matrix[ix][jx];
    sums[jx] = sum;
  }
}

int
main(int argc, char ** argv) {
    // Init and insert values
    struct matrix * mat = matrix_init(SIZE, SIZE);
    mult_table(mat);

    // Calculate row and column sums
    double * sum_rows = (double *)malloc(sizeof(double) * SIZE);
    double * sum_cols = (double *)malloc(sizeof(double) * SIZE);
    row_sums(sum_rows, (const double **)mat->as, SIZE, SIZE);
    /* row_sums(sum_cols, (const double **)mat->as, SIZE, SIZE); */

    // print
    /* vec_print(sum_rows, SIZE); */
    /* printf("\n\n"); */
    /* vec_print(sum_cols, SIZE); */
    /* printf("\n\n"); */
    /* matrix_print(mat); */
    return(0);
}
