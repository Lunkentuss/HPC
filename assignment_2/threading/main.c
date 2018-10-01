#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <getopt.h>
#include <math.h>
#include <complex.h>

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
const int * result_attr;

// Row-major output of the convergence ppm
const int * result_conv;

// Gloval variables for the threads
unsigned int thread_count;
unsigned int num;
pthread_mutex_t lock;
const unsigned int num_per_thread;

// Structs
struct newton_result {
    unsigned int iter_count;
    complex z_conv;
};

struct worker_data {
    unsigned int start, end;
};

// Define functions
void num_to_z(const unsigned int num, complex * result);
complex func(const complex z);
complex func_prime(const complex z);
complex newton_iteration(const complex z);
void newton(const complex z_start, struct newton_result * result);
void new_worker_data(struct worker_data * wd);
void run();

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

complex
func(const complex z)
{
    return(cpow(z, d) - 1);
}

complex
func_prime(const complex z)
{
    return(d * cpow(z, d-1));
}

complex
newton_iteration(const complex z)
{
    complex z_next = z - func(z) / func_prime(z);
    return(z_next);
}

void
newton(const complex z_start, struct newton_result * result)
{
    complex z_k = z_start;
    unsigned int iter_count = 0;

    double z_mag;
    do { 
        z_k = newton_iteration(z_k);
        z_mag = cabs(z_k);
        iter_count++;
    } while(z_mag > END_MAG_LOW && z_mag < END_MAG_HIGH);

    result->z_conv = z_k;
    result->iter_count = iter_count;
    return;
}

/* Function that delegate worker data to the workers
   and is called during mutex. */
void
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

int
main(int argc, char ** argv) {
    result_attr = (int *)malloc(sizeof(int)*1000);
    return(0);
}
