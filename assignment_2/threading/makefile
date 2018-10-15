CC=gcc
FLAGS=-O3
LIBS=-lm -lpthread
INCLUDES=-I./util
DEBUGGING=0

.PHONY : clean, prof

all : newton 

newton : newton.c
	$(CC) newton.c -o newton $(FLAGS) $(LIBS) -D NE_DEBUG=$(DEBUGGING) $(INCLUDES)

prof : newton.c
	$(CC) newton.c -o newton $(FLAGS) $(LIBS) -D NE_DEBUG=$(DEBUGGING) $(INCLUDES) -pg

clean : 
	rm newton *.ppm *.html
	rm *.ppm
