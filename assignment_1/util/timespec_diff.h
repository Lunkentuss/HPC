#ifndef TIMESPEC_DIFF_H
# define TIMESPEC_DIFF_H

#define NANO 1000000000

struct timespect * timespec_diff(struct timespec * start, struct timespec * stop, struct timespec * result){
    if (stop->tv_nsec - start->tv_nsec < 0){
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + NANO;
    }
    else{
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
}

#endif
