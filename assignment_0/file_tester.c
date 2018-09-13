#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

// Creates a matrix struct with entries like a multiplication table specified
// by size.
struct matrix * multiplication_table(int size) {
    struct matrix * mat = matrix_init(size, size);
    for (int i = 1 ; i <= mat->m ; i++)
        for (int j = 1 ; j <= mat->n ; j++)
            mat->as[i-1][j-1] = i * j;

    return(mat);
}

int main() {
    FILE * file;
    char * file_name = "matrix.dat";

    // Open and write to file
    file = fopen(file_name, "w");
    int size = 1000;
    struct matrix * mat = multiplication_table(size);
    write_matrix(file, mat);
    fclose(file);

    // Open and read file
    file = fopen(file_name, "r");
    struct matrix * r_mat = read_matrix(file);
    fclose(file);

    // Test if matrices is equal
    bool is_equal = matrix_equal(mat, r_mat);
    if (is_equal == true) {
        printf("The matrices is equal\n");
    }
    else {
        printf("The matrices is not equal\n");
    }

    return(0);
}
