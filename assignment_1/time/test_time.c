#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "timespec_diff.h"

#define N 400000000

long int sum_numbers(long int n){
    // Naive approach to summing 1...n numbers and will return a
    // overflow result if the return value is bigger than (maximum of long).

    long int sum = 0;
    for(long int i = 1 ; i <= n ; i++){
        sum += i;
    }
    return(sum);
}

int main(int argc, char ** argv) {
    struct timespec start;
    struct timespec end;
    struct timespec result;

    // Get time before loop
    timespec_get(&start, TIME_UTC);

    // Sum numbers to N
    long int sum = sum_numbers(N);

    // Get time after loop
    timespec_get(&end, TIME_UTC);

    // Calculate time difference
    timespec_diff(&start, &end, &result);

    // Print the result
    printf("Sum: %ui\n", sum);
    printf("Time differance:\nSeconds: %d\nNano-seconds: %d\n", result.tv_sec, result.tv_nsec);

    return(0);
}
