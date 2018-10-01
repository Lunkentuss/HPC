#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <getopt.h>
#include <complex.h>
#include <math.h>

#define MIN(x, y) x < y ? x : y
#define POW2(x) x * x

// The size from origo in the resulting pixel map
#define LL 2

// Constants for newton ending conditions
#define END_MAG_LOW 1e-3
#define END_MAG_HIGH 1e10

// Global variables
unsigned int line_count = 1000;

// Function exponent
int d = 2;

// Row-major output of the attractor ppm
int * result_attr = (int *)malloc(sizeof(int) * line_count * line_count);

// Row-major output of the convergence ppm
int * result_conv = (int *)malloc(sizeof(int) * line_count * line_count);

// Gloval variables for the threads
unsigned int thread_count;
unsigned int num;
pthread_mutex_t lock;
const unsigned int num_per_thread;

void
num_to_z(const unsigned int num, complex * result)
{
    const unsigned x_n = num % line_count;
    const unsigned y_n = num / line_count;
    const double x = (double) x_n / line_count;
    const double y = (double) y_n / line_count;
    *result =  -LL + 2 * LL * x / line_count \
               + I * (-LL + 2 * LL * y / line_count);
    return;
}

struct newton_result {
    unsigned int iter_count;
    complex z_conv;
}

void inline
newton(complex * z_start, struct newton_result * result)
{
    complex * z_k = result->z_conv;
    *z_k = *z_start;

    unsigned int * iter_count = &result->iter_count;

    *iter_count = 0;
    do { 
        newton_iteration(z_k);
        (*iter_count)++;
    } while(cabs(z_k) > END_MAG_LOW && z_k < END_MAG_HIGH);
    
    result->iter_count = iter_count;
    return;
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

void inline
newton_iteration(complex * z)
{
    *z = *z - func(z) / func_prime(z));
    return;
}

struct worker_data {
    unsigned int start, end;
}

/* Function that delegate worker data to the workers
   and is called during mutex. */
void inline
new_worker_data(struct worker_data * wd)
{
    unsigned int num_max = POW2(line_count);
    wd->start = num;
    wd->end = MIN(num + num_per_thread, num_max);
    num = MIN(num + num_per_thread + 1, num_max);
    return;
}

void
run() {
    struct worker_data wd;

    // Get the worker data.
    pthread_mutex_lock(&lock);
    new_worker_data(&wd);
    pthread_mutex_unlock(&lock);

    for (int i = wd.start ; i <= wd.end ; i++) {
        printf("Test: %d", i);
    }
}
