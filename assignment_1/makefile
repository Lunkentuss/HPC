CC=gcc

all : alloc_tester file_tester printargs_tester

alloc_tester : alloc_tester.c
	$(CC) -o alloc_tester alloc_tester.c

file_tester : file_tester.c matrix.h
	$(CC) -o file_tester file_tester.c

printargs_tester : printargs_tester.c
	$(CC) -o printargs_tester printargs_tester.c
