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
#define LL 2.0

// Constants for newton ending conditions
#define END_MAG_LOW 1e-3
#define END_MAG_HIGH 1e10

// Global variables
unsigned int LINE_COUNT;

// Function exponent
int D;

// Solutions to the function x^d - 1 = 0
double complex * SOL;

// Row-major output of the attractor ppm
int * RESULT_ATTR;

// Row-major output of the convergence ppm
unsigned int * RESULT_CONV;

/* Gloval variables for the threads */

// The number of threads
unsigned int THREAD_COUNT;

// The bookkeeping index that is used to delegate worker data
bool * JOBS_FINISHED;

// The bookkeeping index that is used to set work jobs to finished
// and is used mainly for the write thread
unsigned int JOB_INDEX;
unsigned int JOB_COUNT;

// The lock for mutex
pthread_mutex_t LOCK;

// The amount of numbers per thread
unsigned int PIXELS_PER_JOB;

// Structs
struct newton_result {
    unsigned int iter_count;
    int solution_index;
};

struct worker_data {
    unsigned int start, end, job_index;
};

struct pixel {
    unsigned int x;
    unsigned int y;
};

// Define functions
complex * solutions(const unsigned dim);
int near_solution_index(const double complex z);
void num_to_z(const unsigned int num, complex * result);
complex func(const complex z);
complex func_prime(const complex z);
complex newton_iteration(const complex z);
void newton(const complex z_start, struct newton_result * result);
void new_worker_data(struct worker_data * wd);
void worker_calc(unsigned int start, unsigned int end);
void * run();

/* Returns the complex solutions to the equation x^d - 1  */
complex *
solutions(const unsigned int dim)
{
    complex * solu = (complex *)malloc(sizeof(complex) * dim);
    for (int i = 0 ; i < dim; i++) {
        solu[i] = cos(2 * M_PI * (double)i / (double)dim) \
                   + I * sin(2 * M_PI * (double)i / (double)dim);
    }
    return(solu);
}

int
near_solution_index(const double complex z)
{
    for (int i = 0 ; i < D ; i++){
        if (cabs(z - SOL[i]) < END_MAG_LOW){
            /* printf("Real: %lf\n", creal(z)); */
            /* printf("Imag: %lf\n", cimag(z)); */
            return(i);
        }
    }
    return(-1);
}

/* The pixels in the resulting pixelmap is numbered in a row-major ordering
   and this function translates the a number to the corresponding one to one
   complex number */
void
num_to_z(const unsigned int p_index, complex * result)
{
    const unsigned x_n = p_index % LINE_COUNT;
    const unsigned y_n = p_index / LINE_COUNT;
    const double x = (double) x_n / (double) LINE_COUNT;
    const double y = (double) y_n / (double) LINE_COUNT;
    *result =  -LL + 2 * LL * x \
               + I * (-LL + 2 * LL * y);
    return;
}

/* Calculates the value of z^d - 1  */
double complex
func(const complex z)
{
    return(cpow(z, D) - 1);
}

/* Calculates the value of the prime of z^d - 1  */
double complex
func_prime(const double complex z)
{
    return(D * cpow(z, D-1));
}

/* Returns the result of a newton iteration  */
double complex
newton_iteration(const double complex z)
{
    double complex z_next = z - func(z) / func_prime(z);
    return(z_next);
}

void
newton(const double complex z_start, struct newton_result * result)
{
    double complex z_k = z_start;
    unsigned int iter_count = 0;
    int solution_index;

    double z_mag;
    do { 
        /* printf("%lf\n", creal(z_k)); */
        /* printf("%lf\n", cimag(z_k)); */
        z_k = newton_iteration(z_k);
        z_mag = cabs(z_k);
        iter_count++;
        solution_index = near_solution_index(z_k);
    } while(z_mag > END_MAG_LOW && z_mag < END_MAG_HIGH \
            && solution_index == -1);
    /* printf("Ended ====\n"); */

    result->solution_index = solution_index;
    result->iter_count = iter_count;
    return;
}

/* Function that delegate worker data to the workers
   and is called during mutex.  */
void
new_worker_data(struct worker_data * wd)
{
    wd->start = JOB_INDEX * PIXELS_PER_JOB;
    wd->end = MIN((JOB_INDEX + 1) * PIXELS_PER_JOB, POW2(LINE_COUNT));
    wd->job_index = JOB_INDEX;
    JOB_INDEX++;
    return;
}

/* Thread worker function  */
void *
run() {
    struct worker_data wd;

    bool continue_loop = true;

    while(continue_loop){
        // Get the worker data during mutex lock
        pthread_mutex_lock(&LOCK);
        new_worker_data(&wd);
        pthread_mutex_unlock(&LOCK);

        // Do worker calculation until all calculations
        // have been calculated.
        if(wd.start >= wd.end)
            continue_loop = false;
        else{
            worker_calc(wd.start, wd.end);
            JOBS_FINISHED[wd.job_index] = true;
            /* printf("Job index: %d\n", wd.job_index); */
        }
    }
    return NULL;
}

/* This function does all the worker calculations  */
void
worker_calc(const unsigned int start, const unsigned int end)
{
    for (int i = start ; i < end ; i++) {
        double complex z_start;
        double complex * z_startPtr = &z_start;
        struct newton_result result;
        struct newton_result * resultPtr = &result;

        num_to_z(i, z_startPtr);
        /* printf("Real: %lf\n", creal(z_start)); */
        /* printf("Imag: %lf\n", cimag(z_start)); */
        newton(z_start, resultPtr);

        RESULT_ATTR[i] = resultPtr->solution_index;
        RESULT_CONV[i] = resultPtr->iter_count;

        /* printf("%d\n", resultPtr->solution_index); */
        /* printf("%d\n", resultPtr->iter_count); */
    }
}

void *
worker_write()
{
    printf("Write worker started\n");
}

int
main(int argc, char ** argv) {
    LINE_COUNT = 5;
    THREAD_COUNT = 1;
    PIXELS_PER_JOB = 2;
    D = 3;

    JOB_INDEX = 0;
    JOB_COUNT = POW2(LINE_COUNT) / PIXELS_PER_JOB;
    // Compensate for eventual truncation
    if (POW2(LINE_COUNT) % PIXELS_PER_JOB != 0)
        JOB_COUNT++;

    JOBS_FINISHED = (bool *)calloc(JOB_COUNT, sizeof(bool));
    SOL = (complex *)malloc(sizeof(complex) * D);
    SOL = solutions(D);
    RESULT_ATTR = (int *)malloc(sizeof(unsigned int)*POW2(LINE_COUNT));
    RESULT_CONV = (int *)malloc(sizeof(int)*POW2(LINE_COUNT));

    // Create threads
    pthread_t threads[THREAD_COUNT];
    for(int i = 0 ; i < THREAD_COUNT ; i++){
        if(pthread_create(&threads[i], NULL, run, (void*)NULL)){
            printf("Creation of thread %d failed\n", i);
        }
    }

    // Create write thead
    pthread_t write_thread;
    pthread_create(&write_thread, NULL, worker_write, (void*)NULL);

    // Join the threads
    for(int i = 0 ; i < THREAD_COUNT ; i++){
        pthread_join(threads[i], NULL);
    }
    pthread_join(write_thread, NULL);

    printf("Finished\n");

    free(JOBS_FINISHED);
    free(SOL);
    free(RESULT_ATTR);
    free(RESULT_CONV);

    return(0);
}
