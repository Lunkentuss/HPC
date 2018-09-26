CC=gcc
FLAGS=-O2
I=-I.

all : inlined mainfile seperatefile

mainfile : mainfile.c
	$(CC) $(FLAGS) -o mainfile 

seperatefile : seperatefile.c seperate_file.h
	$(CC) $(FLAGS) -o seperatefile 

inlined : inlined.c
	$(CC) $(FLAGS) -o inlined

