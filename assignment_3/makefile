CC=gcc
FLAGS=-fopenmp -O3 -march=native
INCLUDES=-Iutil
LIBS=-lm


all : main

.PHONY : clean

main : main.c
	$(CC) main.c $(FLAGS) -o main $(INCLUDES) $(LIBS)

clean : 
	rm main
