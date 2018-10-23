CC=gcc
FLAGS=-fopenmp -O3 -march=native


all : test

.PHONY : clean

test : test.c
	$(CC) test.c $(FLAGS) -o test

clean : 
	rm test
