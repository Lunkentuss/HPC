#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000000000

void
allocate(int * int_ptr, const int n)
{
	int_ptr = (int *)malloc(sizeof(int) * n);
}

int
sum(const int n)
{
	int sum = 0;
	for (int i = 1 ; i <= n ; i++)
		sum += i;

	return sum;
}

int main(double argc, char ** argv) {
	int * int_ptr;
	allocate(int_ptr, 1000);
	sum(SIZE);
	free(int_ptr);
	return(0);
}
