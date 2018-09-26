CC=gcc
FLAGS=-O2
I=-I.

all : inlined mainfile seperatefile

mainfile : mainfile.c
	$(CC) mainfile.c $(FLAGS) -o mainfile 

seperatefile : seperatefile.c seperate_file.h
	$(CC) seperatefile.c $(FLAGS) -o seperatefile $(I)

inlined : inlined.c
	$(CC) inlined.c $(FLAGS) -o inlined
