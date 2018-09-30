#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <bool.h>
#include <getopt.h>
#include <complex.h>
#include <math.h>

// The size from origo in the resulting pixel map
#define LL 2

// Constants for newton ending conditions
#define END_MAG_LOW = 1e-3;
#define END_MAG_HIGH = 1e10;

// Global variables
unsigned int line_count = 1000;

// Function exponent
int d = 2;

// Row-major output of the attractor ppm
int * result_attr = malloc(sizeof(int) * line_count * line_count);

// Row-major output of the convergence ppm
int * result_conv = malloc(sizeof(int) * line_count * line_count);

// Gloval variables for the threads
unsigned int thread_count;
unsigned int num;
const unsigned int num_per_thread;

void
num_to_z(const unsigned int num, complex * result)
{
    const unsigned x_n = num % line_count;
    const unsigned y_n = num / line_count;
    const double x = (double) x_n / line_count;
    const double y = (double) y_n / line_count;
    complex z;
    z =  -LL + 2 * LL * x / line_count \
         + I * (-LL + 2 * LL * y / line_count);
    return(z);
}

struct newton_result {
    unsigned int iter_count;
    complex z_conv;
}

void inline
newton(const complex * z_start, struct * newton result)
{
    z_k = *z_start;

    unsigned int iter_count = 0;

    do { 
        z_k = newton_iteration(z_k);
    } while(cabs(z_k) > END_MAG_LOW && z_k < END_MAG_HIGH);
    
    result->iter_count = iter_count;
    result->z_conv = z_k;
    return;
}

complex inline
newton_iteration(const complex z)
{
    return(z - func(z) / func_prime(z));
}

complex inline
func(const complex z)
{
    return(cpow(z, d) - 1);
}

complex inline
func_prime(const complex z)
{
    return(d * cpow(z, d-1));
}

int
main(int argc, char ** argv) {
    return(0);
}

struct worker_data {
    unsigned int start, end;
}

/* Function that delegate worker data to the workers
   and is called during mutex. */
void inline
next_worker_data(struct worker_data * wd)
{
    wd->start = num;
    wd->end = num + num_per_thread;
    num = num + num_per_thread + 1;
    return;
}
