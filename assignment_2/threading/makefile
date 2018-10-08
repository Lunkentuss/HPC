CC=gcc
FLAGS=-O0
LIBS=-lm -lpthread
DEBUGGING=0

all : newton 

newton : newton.c
	$(CC) newton.c -o newton $(FLAGS) $(LIBS) -D NE_DEBUG=$(DEBUGGING)
