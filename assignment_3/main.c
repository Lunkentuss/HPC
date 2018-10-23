#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int main(int argc, char ** argv)
{
    omp_set_num_threads(2);
    #pragma opm parallel
    {
        printf("Hejsan\n");
    }
    int a = omp_get_num_threads();
    printf("%d\n", a);
}
