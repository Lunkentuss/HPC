CC=gcc
FLAGS=-O3
LIBS=-lm -lpthread
INCLUDES=-I./util
DEBUGGING=0

.PHONY : clean, prof, debug

all : newton 

newton : newton.c
	$(CC) newton.c -o newton $(FLAGS) $(LIBS) -D NE_DEBUG=$(DEBUGGING) $(INCLUDES)

prof : newton.c
	$(CC) newton.c -o newton $(FLAGS) $(LIBS) -D NE_DEBUG=$(DEBUGGING) $(INCLUDES) -pg

debug : newton.c
	$(CC) newton.c -o newton $(FLAGS) $(LIBS) -D NE_DEBUG=$(DEBUGGING) $(INCLUDES) -g

clean : 
	rm newton *.ppm *.html
	rm *.ppm
