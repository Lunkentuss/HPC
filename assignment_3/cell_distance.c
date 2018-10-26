#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <getopt.h>
#include "block.h"
#include "fixed_point.h"
#include "point.h"

#define SIZE_COUNT 3465
#define SIZE_BLOCK_READ 1000000
#define SIZE_VALUE_STR 8
#define SIZE_POINT_STR 24
#define OUTPUT_FACT 100

/* Global variables */
int THREAD_COUNT;
int count[SIZE_COUNT];

/* Function declerations */
void usage();
struct cell_point * read_points(struct block_t block);
inline int parse_value(char * valuePtr);
inline void parse_point(char * pointPtr, struct cell_point * point);
inline float distance(const struct cell_point p1, const struct cell_point p2);

void
usage()
{
    printf("Usage\n");
}

struct cell_point *
read_points(struct block_t block)
{
    int read_count = SIZE_BLOCK(block);

    struct block_t * blocks = devide_block(block, THREAD_COUNT);
    char * points_str = (char *) \
            malloc(sizeof(char) * SIZE_POINT_STR * read_count);

    #pragma omp parallel
    {
        int thread_idx = omp_get_thread_num();
        struct block_t block_ = blocks[thread_idx];
        FILE * file;
        file = fopen("cells", "r");
        fseek(file, sizeof(char) * block_.idx1 * SIZE_POINT_STR, SEEK_SET);

        int start = SIZE_POINT_STR * (block_.idx1 - block.idx1);
        fread(
            &points_str[SIZE_POINT_STR * (block_.idx1 - block.idx1)],
            sizeof(char),
            SIZE_POINT_STR * SIZE_BLOCK(block_),
            file);
        fclose(file);
    }

    struct cell_point * cell_points = (struct cell_point *) malloc(
        sizeof(struct cell_point) * read_count);
    #pragma omp parallel for
    for (int i = 0 ; i < read_count ; i++){
        parse_point(&points_str[i * SIZE_POINT_STR], &cell_points[i]);
    }

    // Free
    free(blocks);
    free(points_str);
    return cell_points;
}

inline int
parse_value(char * valuePtr)
{
    int value = 0;
    value += (int)(valuePtr[1] - '0') * (FIXED_FACT * 10);
    value += (int)(valuePtr[2] - '0') * (FIXED_FACT * 1);
    // Skip
    value += (int)(valuePtr[4] - '0') * (FIXED_FACT / 10);
    value += (int)(valuePtr[5] - '0') * (FIXED_FACT / 100);
    value += (int)(valuePtr[6] - '0') * (FIXED_FACT / 1000);
    if (valuePtr[0] == '-'){
        value *= -1;
    }
    return value;
}

inline void
parse_point(char * pointPtr, struct cell_point * point)
{
    point->x = parse_value(&pointPtr[0 * SIZE_VALUE_STR]);
    point->y = parse_value(&pointPtr[1 * SIZE_VALUE_STR]);
    point->z = parse_value(&pointPtr[2 * SIZE_VALUE_STR]);
}

inline float
distance(const struct cell_point p1, const struct cell_point p2)
{
    int sum = 0;

    int diff_x = p1.x - p2.x;
    sum += diff_x * diff_x;

    int diff_y = p1.y - p2.y;
    sum += diff_y * diff_y;

    int diff_z = p1.z - p2.z;
    sum += diff_z * diff_z;

    float sum_f = FIXED_TO_FLOAT(sum) / FIXED_FACT;
    float dist = sqrtf(sum_f);
    return dist;
}

inline int
dist_to_idx(const float dist)
{
    int idx = (int)(dist * OUTPUT_FACT + 0.5);
    return idx;
}

inline void
combinatorial_self(const struct cell_point * points, const int num_points)
{
    #pragma omp parallel for schedule(dynamic, THREAD_COUNT) reduction(+:count)
    for(int i = 0 ; i < num_points ; i++){
        for(int j = i+1 ; j < num_points ; j++){
            float dist = distance(points[i], points[j]);
            size_t idx = dist_to_idx(dist);
            count[idx] += 1;
        }
    }
}

inline void
combinatorial_other(
    struct cell_point * points_1,
    int num_points_1,
    struct cell_point * points_2,
    int num_points_2)
{
    #pragma omp parallel for schedule(dynamic, THREAD_COUNT) reduction(+:count)
    for(int i = 0 ; i < num_points_1 ; i++){
        for(int j = 0 ; j < num_points_2 ; j++){
            float dist = distance(points_1[i], points_2[j]);
            size_t idx = dist_to_idx(dist);
            count[idx] += 1;
        }
    }
}

void
count_distances(const struct block_t block)
{
    int size_block = SIZE_BLOCK(block);
    int num_blocks = size_block / SIZE_BLOCK_READ;
    if(size_block % SIZE_BLOCK_READ != 0)
        num_blocks++;

    struct block_t * blocks = devide_block(block, num_blocks);

    for(int i = 0 ; i < num_blocks ; i++){
        struct cell_point * points_i = read_points(blocks[i]);
        int size_i = SIZE_BLOCK(blocks[i]);
        combinatorial_self(points_i, size_i);

        for(int j = i + 1 ; j < num_blocks ; j++){
            struct cell_point * points_j = read_points(blocks[j]);
            int size_j = SIZE_BLOCK(blocks[j]);
            combinatorial_other(points_i, size_i, points_j, size_j);
            free(points_j);
        }
        free(points_i);
    }
    free(blocks);
}

void
print_distances()
{
    int dist_integral = 0;
    int dist_decimal = 0;
    for (int i = 0 ; i < SIZE_COUNT; i++)
    {
        if (count[i] != 0){
            printf("%0*d.%0*d", 2, dist_integral, 2, dist_decimal);
            printf(" %d\n", count[i]);
        }
        dist_decimal++;
        dist_decimal = dist_decimal % 100;
        if (dist_decimal == 0){
            dist_integral++;
        }
    }
}

int main(int argc, char ** argv)
{
    for (int i = 0 ; i < SIZE_COUNT ; i++){
        count[i] = 0;
    }

    THREAD_COUNT = 1;
    int c;
    while((c = getopt_long(argc, argv, "t:", NULL, NULL))
           != -1){
        switch(c)
        {
            case 't':
                THREAD_COUNT = atoi(optarg);
                break;

            default:
                usage();
                break;
        }
    }

    omp_set_num_threads(THREAD_COUNT);
    struct block_t b;
    FILE * file = fopen("cells", "r");
    fseek(file, 0, SEEK_END);

    b.idx1 = 0;
    b.idx2 = ftell(file) / SIZE_POINT_STR;

    count_distances(b);
    print_distances();

    return 0;
}
