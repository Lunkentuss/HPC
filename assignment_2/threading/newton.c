#include <complex.h>
#include <getopt.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "exp_by_square.h"

/* 
 * Macros 
 */

#define MIN(x, y) x < y ? x : y
#define POW2(x) (x) * (x)
#define C_SQUARE_MAG(c) creal(c) * creal(c) + cimag(c) * cimag(c)
#define C_TYPE double

// Debug logging
#ifndef NE_DEBUG
# define NE_DEBUG 0
#endif

#if NE_DEBUG == 1
# define LOG(x, ...) printf(x, __VA_ARGS__)
#else
# define LOG(x, ...)
#endif

// The maximum distance from origo for the coordinates
#define LL 2.0

// Constants used for terminating Newton's Method
#define END_MAG_LOW 1E-3
#define END_MAG_HIGH 1E10

// The maximum number of digits allowed as a color value for convergences
#define MAX_CONV_CHAR_SIZE 3
#define MAX_CONV_NBR 999

/* 
 * Global variables
 */

// The resolution of the image
unsigned int LINE_COUNT;

// The exponent for our given function
int D;

// Newton's Method function
double complex (*NEWTON_FUNC)(double complex);

// "Exponential by squaring" function for calculating pow(z, D - 1)
double complex (*POW_FUNC)(double complex);

// Attractor ppm_pixel
char **attr_ppm_pixels;

// Solutions to the given equation
double complex *SOL;

// Row-major output of the attractor ppm
int *RESULT_ATTR;

// Row-major output of the convergence ppm
unsigned int *RESULT_CONV;

/* 
 * Global variables (Threads)
 */

// The number of threads
unsigned int THREAD_COUNT;

// Used to keep track of which jobs have been performed
bool *JOBS_FINISHED;

// The total number of jobs (based on a set job size)
unsigned int JOB_COUNT;

// Used to keep track of which job is the next available one 
unsigned int JOB_INDEX;

// The lock for updating the job index
pthread_mutex_t LOCK;

// The job size
unsigned int PIXELS_PER_JOB;

// The sleep waiting time for the write thread
long SLEEP_TIME_NANO;

/*
 * Structs
 */

struct newton_result {
    unsigned int iter_count;
    int solution_index;
};

struct worker_data {
    unsigned int start, end, job_index;
};

/* 
 * Function declarations 
 */

// Miscellaneous
void usage();
int digit_count(int number);

// Numerical (Helper)
void set_newton_func();
void set_pow_func();
double complex newton_iteration(const double complex z);
double complex newton_iterationD1(const double complex z); 

// Numerical
double complex *get_solutions(const unsigned dim);
int near_solution_index(const double complex z);
void index_to_z(const unsigned int num, double complex * result);
void newton(const double complex z_start, struct newton_result * result);

// Compute threads
void * run_compute();
void acquire_job(struct worker_data *wd);
void perform_job(unsigned int start, unsigned int end);

// Write thread
void * run_write();
void write_header(FILE *file_attr, FILE *file_conv);
char ** generate_colors();
void write_pixels(unsigned int start, unsigned int end, FILE *file_attr,
    FILE *file_conv, char **attr_colors, int attr_pixel_size);

/* 
 * Functions 
 */

/* Prints flag instructions */
void
usage()
{
    printf("\
    Synopsis: newton [OPTIONS] dim \n\
    \n\
    Where dim is the d=dim of the polynom x^d - 1 \n\
    \n\
    OPTIONS: \n\
    \t-t n : Set the number of threads\n\
    \t-l n : Set the number of lines \n\
    \t-s n : Set the waiting time in nano seconds for the write thread \n\
    \t-j n : Set the number of jobs per thread at one time \n\
    ");
    
    exit(EXIT_FAILURE);
}

/* Returns the number of digits for a decimal numeral */
int
digit_count(int number)
{
    int count = 0;

    while(number != 0)
    {
        count++;
        number /= 10;
    }

    return count;
}

/* Heuristic function for generating different color values */
char**
generate_colors()
{
    int dim_digit_count = digit_count(D + 2);
    int pixel_size = (dim_digit_count + 1) * 3;

    char *values = (char *)malloc(sizeof(char) * pixel_size * (D + 1));
    char **array = (char **)malloc(sizeof(char *) * (D + 1));

    // Infinite convergence has color black
    *array = values;
    for(int i = 0; i < 3; i++) {
        char zeros[dim_digit_count + 2];
        sprintf(zeros, "%0*d ", dim_digit_count, 0);
        memcpy(values + i * (dim_digit_count + 1), zeros, dim_digit_count + 1);
    }

    // Generate attractor colors
    int pos = 0;
    int base_value = 0;
    for (int i = 1; i <= D; i++) {
        *(array + i) = values + i * pixel_size;
        // Iterate over RGB values
        for (int j = 0 ; j < 3 ; j++) {
            int sat = base_value + (j + pos) % 3;
            char sat_s[dim_digit_count + 2];
            sprintf(sat_s, "%0*d ", (unsigned short)dim_digit_count, sat);
            memcpy(
                values + i * pixel_size + j * (dim_digit_count + 1),
                sat_s,
                dim_digit_count + 1);
        }
        pos = (pos + 1) % 3;
        base_value++;
    }

    return(array);
}

/* Returns the complex solutions to the given equation */
double complex*
get_solutions(const unsigned int dim)
{
    double complex * solu = (double complex *)malloc(sizeof(double complex) * dim);
    for (int i = 0; i < dim; i++) {
        solu[i] = cos(2 * M_PI * (double)i / (double)dim) \
                   + I * sin(2 * M_PI * (double)i / (double)dim);
    }

    return(solu);
}

/* Checks if the given iteration fulfills convergence criterion */
int
near_solution_index(const double complex z)
{
    LOG("\t\t === Solutions ===", ' ');
    LOG("\t\tReal: %lf\n", creal(z));
    LOG("\t\tImag: %lf\n", cimag(z));
    for (int i = 0 ; i < D ; i++)
        if (C_SQUARE_MAG(z - SOL[i]) < POW2(END_MAG_LOW))
            return i;
        
    return -1;
}

/* Transforms vector index to corresponding complex number in a discrete grid */
void
index_to_z(const unsigned int p_index, double complex *result)
{
    const unsigned i = p_index % LINE_COUNT;
    const unsigned j = p_index / LINE_COUNT;
    const double x = (double)i / (double)LINE_COUNT;
    const double y = (double)j / (double)LINE_COUNT;
    *result =  -LL + 2 * LL * x \
               + I * (LL - 2 * LL * y);
    
    return;
}

/* Returns the result of iterating with Newton's Method once */
double complex
newton_iteration(const double complex z)
{
    double complex z_next;
    z_next = z - z / D + 1 / (D * POW_FUNC(z));
    return(z_next);
}

double complex
newton_iterationD1(const double complex z)
{
    double complex z_next = 1;
    return(z_next);
}

void
set_newton_func()
{
    if(D == 1){
        NEWTON_FUNC = newton_iterationD1;
    }
    else{
        NEWTON_FUNC = newton_iteration;
    }
}

/* Delegates the corresponding function for calculating D-1 */
void
set_pow_func()
{
    switch(D){
        case 2:
            POW_FUNC = pow_square_1;
            break;
        case 3:
            POW_FUNC = pow_square_2;
            break;
        case 5:
            POW_FUNC = pow_square_4;
            break;
        case 7:
            POW_FUNC = pow_square_6;
            break;
        default:
            POW_FUNC = pow_square_generic_fixed;
            FIXED_POWER = D - 1;
            break;
        }
}

inline void
newton(const double complex z_start, struct newton_result *result)
{
    double complex z_k = z_start;
    unsigned int iter_count = 0;

    LOG("=== New newton iteration ===\n", ' ');
    
    int solution_index = -1;
    double z_mag_square = C_SQUARE_MAG(z_k);
    while(
            z_mag_square > POW2(END_MAG_LOW)     // Check lower bound
            && z_mag_square < POW2(END_MAG_HIGH) // Check upper bound
            && solution_index == -1)             // Check if near solution
    {
        LOG("\n\t=== Newton iteration ===\n", ' ');
        LOG("\t%lf\n", creal(z_k));
        LOG("\t%lf\n", cimag(z_k));
        z_k = NEWTON_FUNC(z_k);
        z_mag_square = C_SQUARE_MAG(z_k);
        iter_count++;
        solution_index = near_solution_index(z_k);
    }

    result->solution_index = solution_index;
    result->iter_count = iter_count;
    return;
}

/* Function that delegate worker data to the workers
   and is called during mutex */
void inline
acquire_job(struct worker_data * wd)
{
    wd->start = JOB_INDEX * PIXELS_PER_JOB;
    wd->end = MIN((JOB_INDEX + 1) * PIXELS_PER_JOB, POW2(LINE_COUNT));
    wd->job_index = JOB_INDEX;
    JOB_INDEX++;
    return;
}

/* Thread worker function */
void *
run_compute() {
    struct worker_data wd;

    bool continue_loop = true;

    while(continue_loop){
        // Get the worker data during mutex lock
        pthread_mutex_lock(&LOCK);
        acquire_job(&wd);
        pthread_mutex_unlock(&LOCK);

        // Do worker calculation until all calculations
        // have been calculated.
        if(wd.start >= wd.end)
            continue_loop = false;
        else{
            perform_job(wd.start, wd.end);
            JOBS_FINISHED[wd.job_index] = true;
            /* printf("Job index: %d\n", wd.job_index); */
        }
    }
    return NULL;
}

/* Perform calculations for each pixel part of a specified job */
void inline
perform_job(const unsigned int start, const unsigned int end)
{
    for (int i = start; i < end; i++) {
        double complex z_start;
        double complex * z_startPtr = &z_start;
        struct newton_result result;
        struct newton_result * resultPtr = &result;

        index_to_z(i, z_startPtr);

        LOG("=== Z-start ===\n", ' ');
        LOG("Real: %lf\n", creal(z_start));
        LOG("Imag: %lf\n", cimag(z_start));

        newton(z_start, resultPtr);

        // Store results
        RESULT_ATTR[i] = resultPtr->solution_index;
        RESULT_CONV[i] = resultPtr->iter_count;

        /* printf("%d\n", resultPtr->solution_index); */
        /* printf("%d\n", resultPtr->iter_count); */
    }
}

/* Used by writing thread to wait for jobs to be completed */
void inline
wait_for_job(const unsigned int job_index)
{
    while(true) {
        if(JOBS_FINISHED[job_index])
            return;
        
        struct timespec sleep_time;
        sleep_time.tv_nsec = SLEEP_TIME_NANO;
        sleep_time.tv_sec = 0;
        nanosleep(&sleep_time, NULL);
    }

    return;
}

/* Writes the result of a single job (in terms of pixels) to files */
void inline
write_pixels(unsigned int start, unsigned int end, FILE *file_attr,
    FILE *file_conv, char **attr_colors, int attr_pixel_size)
{
    for(int i = start; i < end; i++) {
        if (i % LINE_COUNT == 0) {
            char nl = '\n';
            fwrite(&nl, sizeof(char), 1, file_attr);
            fwrite(&nl, sizeof(char), 1, file_conv);
        }

        // Print to attractor file
        fwrite(attr_colors[RESULT_ATTR[i] + 1], attr_pixel_size, 1, file_attr);

        // Print to convergence file
        /* RESULT_CONV[i] = MIN(MAX_CONV_NBR, RESULT_CONV[i]); */

        char output_conv[MAX_CONV_CHAR_SIZE + 2];
        sprintf(output_conv, "%0*d ", MAX_CONV_CHAR_SIZE, RESULT_CONV[i]);

        // Iterate and print equal RGB values (for gray scale)
        for (int i = 0; i < 3; i++)
            fwrite(output_conv, MAX_CONV_CHAR_SIZE + 1, 1, file_conv);
    }

    return;
}

void *
run_write()
{
    char file_name_attr[30];
    char file_name_conv[30];
    sprintf(file_name_attr, "newton_attractors_x%d.ppm", D);
    sprintf(file_name_conv, "newton_convergence_x%d.ppm", D);
    FILE * file_attr = fopen(file_name_attr, "w");
    FILE * file_conv = fopen(file_name_conv, "w");

    // Write header to file
    write_header(file_attr, file_conv);

    // Generate colors for the attractors
    char **attr_colors = generate_colors();
    int attr_pixel_size = (digit_count(D + 2) + 1) * 3;

    // Start writing
    /* printf("Write worker started\n"); */
    for (int i = 0 ; i < JOB_COUNT ; i++) {
        wait_for_job(i);
        unsigned int start = i * PIXELS_PER_JOB;
        unsigned int end = \
            MIN((i + 1) * PIXELS_PER_JOB, POW2(LINE_COUNT));

        write_pixels(
            start,
            end,
            file_attr, 
            file_conv,
            attr_colors,
            attr_pixel_size);
    }

    fclose(file_attr);
    fclose(file_conv);

    free(attr_colors);
}

void inline
write_header(FILE *file_attr, FILE *file_conv)
{
    fprintf(file_attr, "P3\n");
    fprintf(file_attr, "%d %d\n", LINE_COUNT, LINE_COUNT);
    fprintf(file_attr, "%d", D + 1);
    fprintf(file_conv, "P3\n");
    fprintf(file_conv, "%d %d\n", LINE_COUNT, LINE_COUNT);
    fprintf(file_conv, "%d", 100);
}

int
main(int argc, char **argv) {
    
    // Default parameters
    LINE_COUNT = 2000;
    THREAD_COUNT = 1;
    PIXELS_PER_JOB = 0;
    SLEEP_TIME_NANO = 100000;
    D = 3;

    // One positional argument
    argc--;
    if (argc == 0)
        usage();

    // Parse arguments
    int c;
    while((c = getopt_long(argc, argv, "t:l:j:s:", NULL, NULL))
           != -1){
        switch(c)
        {
            case 't':
                THREAD_COUNT = atoi(optarg);
                break;

            case 'l':
                LINE_COUNT = atoi(optarg);
                break;

            case 'j':
                PIXELS_PER_JOB = atoi(optarg);
                break;

            case 's':
                SLEEP_TIME_NANO = atoi(optarg);
                break;

            default:
                usage();
                break;
        }
    }

    D = atoi(argv[argc]);

    // Set default value for PIXELS_PER_JOB if -j flags is not set
    if (PIXELS_PER_JOB == 0)
        PIXELS_PER_JOB = LINE_COUNT;

    // Debugging
    printf("Lines: %d\n", LINE_COUNT);
    printf("Threads: %d\n", THREAD_COUNT);
    printf("Dimension: %d\n", D);
    printf("Pixel jobs per thread: %d\n", PIXELS_PER_JOB);
    printf("Write sleep time: %ld\n", SLEEP_TIME_NANO);

    // Set job parameters
    JOB_INDEX = 0;
    JOB_COUNT = POW2(LINE_COUNT) / PIXELS_PER_JOB;
    
    // Compensate for eventual truncation
    if (POW2(LINE_COUNT) % PIXELS_PER_JOB != 0)
        JOB_COUNT++;

    JOBS_FINISHED = (bool *)calloc(JOB_COUNT, sizeof(bool));
    RESULT_ATTR = (int *)malloc(sizeof(int)*POW2(LINE_COUNT));
    RESULT_CONV = (unsigned int *)malloc(sizeof(unsigned int)*POW2(LINE_COUNT));
    SOL = get_solutions(D);

    // Sets the newton callback function
    set_newton_func();
    set_pow_func();

    // Create compute threads
    pthread_t threads[THREAD_COUNT];
    for(int i = 0; i < THREAD_COUNT; i++) {
        if (pthread_create(&threads[i], NULL, run_compute, (void*)NULL)) {
            printf("Creation of thread %d failed\n", i);
        }
    }

    // Create write thead
    pthread_t write_thread;
    pthread_create(&write_thread, NULL, run_write, (void *)NULL);

    // Join compute threads
    for(int i = 0 ; i < THREAD_COUNT ; i++)
        pthread_join(threads[i], NULL);
    printf("Compute threads finished\n");

    // Join write thread
    pthread_join(write_thread, NULL);
    printf("Write thread finished\n");

    printf("Finished\n");

    // Clean up
    free(JOBS_FINISHED);
    free(SOL);
    free(RESULT_ATTR);
    free(RESULT_CONV);

    return(0);
}

