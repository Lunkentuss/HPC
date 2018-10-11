CC=gcc
FLAGS=-O0
LIBS=-lm -lpthread
INCLUDES=-I./util
DEBUGGING=0

.PHONY : clean

all : newton 

newton : newton.c
	$(CC) newton.c -o newton $(FLAGS) $(LIBS) -D NE_DEBUG=$(DEBUGGING) $(INCLUDES)

clean : 
	rm newton
