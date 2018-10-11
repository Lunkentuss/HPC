CC=gcc
FLAGS=-O3
LIBS=-lm -lpthread
DEBUGGING=0

.PHONY : clean

all : newton 

newton : newton.c
	$(CC) newton.c -o newton $(FLAGS) $(LIBS) -D NE_DEBUG=$(DEBUGGING)

clean : 
	rm newton
