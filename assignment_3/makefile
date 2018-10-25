CC=gcc
FLAGS=-fopenmp -O3 -march=native
INCLUDES=-Iutil
LIBS=-lm


all : cell_distance

.PHONY : clean

cell_distance : cell_distance.c
	$(CC) cell_distance.c $(FLAGS) -o cell_distance $(INCLUDES) $(LIBS)

clean : 
	rm cell_distance
