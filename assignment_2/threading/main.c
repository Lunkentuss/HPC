#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <getopt.h>
#include <math.h>
#include <complex.h>

#define MIN(x, y) x < y ? x : y
#define POW2(x) x * x
#define C_TYPE double

// The size from origo in the resulting pixel map
#define LL 2

// Constants for newton ending conditions
#define END_MAG_LOW 1e-3
#define END_MAG_HIGH 1e10

// Global variables
unsigned int line_count;

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
unsigned int num_per_thread;

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
void worker_calc(unsigned int start, unsigned int end);
void * run();

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

void *
run() {
    struct worker_data wd;

    bool continue_loop = true;

    while(continue_loop){
        // Get the worker data during mutex lock
        pthread_mutex_lock(&lock);
        new_worker_data(&wd);
        pthread_mutex_unlock(&lock);

        // Do worker calculation until all calculations
        // have been calculated.
        if(wd.start == wd.end)
            continue_loop = false;
        else
            worker_calc(wd.start, wd.end);
    }
}

void
worker_calc(const unsigned int start, const unsigned int end)
{
    for (int i = start ; i <= end ; i++) {
        printf("Test: %d\n", i);
    }
}

int
main(int argc, char ** argv) {
    result_attr = (int *)malloc(sizeof(int)*1000);
    result_conv = (int *)malloc(sizeof(int)*1000);

    line_count = 100;
    num = 0;
    thread_count = 5;
    num_per_thread = 10;

    pthread_t threads[thread_count];

    // Create threads
    for(int i = 0 ; i < thread_count ; i++){
        if(pthread_create(&threads[i], NULL, run, (void*)NULL))
            printf("Creation of thread %d failed\n", i);
    }

    // Join the threads
    for(int i = 0 ; i < thread_count ; i++){
        pthread_join(threads[i], NULL);
    }
    printf("Finished\n");

    return(0);
}
