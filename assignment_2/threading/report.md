# Assignment 2 (Threads)

## Program layout

The solutions, attractors, convergences and most constants are declared as global variables. Structs are used to pass the results from Newton\'s Method and to pass job data to compute threads.

### Main
1. Parameters are set from arguments (using getopt)
    1. -t (THREAD COUNT, DEFAULT: 1)
    2. -l (LINE COUNT, DEFAULT: 2000)
    3. -j (PIXELS PER JOB, DEFAULT: LINE COUNT)
    4. -s (SLEEP TIME IN NANOSECONDS, DEFAULT: 1000000)
    5. (last) (DEGREE, DEFAULT: 3) 
2. Since the work is going to be divided into chunks of \[PIXELS PER JOB\] size, additional parameters related to this are set
3. The roots to the equation are found
4. Memory is allocated
5. Threads are created
6. Threads are joined
7. Memory is freed

### Compute threads
1. Based on the job size, we will have some number of jobs. These jobs are referred to by an index.
2. Each compute thread will try to access a mutex to read this index (and increment it for the next accessor).
3. This index corresponds to a segment of the arrays at which work will be performed.
4. Once that piece of work is performed, we mark that index to be \"done\".
5. As long as there is work, the threads will be keep going.

### Write thread
1. The files are created.
2. Headers are written to the files.
3. Colors are generated for later.
4. We loop through every finished job - writing the colors corresponding to data from those jobs to the file. If we encounter a job that hasn\'t been done yet, we sleep for a while.
5. This is repeated until there are no more jobs.

## Performance aspects

