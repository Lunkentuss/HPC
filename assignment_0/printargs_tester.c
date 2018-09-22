#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARG_COUNT 5
#define BASE 10

void run_program(char * argv[]) {
    long a, b;

    // Check if flags are equal
    if (strcmp(argv[1], argv[3]) == 0) {
        printf("Equal flags are not allowed.\n");
        return;
    }

    int i = 1;
    while (i < ARG_COUNT) {
        char * flag = argv[i];
        char * value = argv[i+1];
        char * end_ptr;
        long value_l = strtol(value, &end_ptr, BASE);

        if (*end_ptr != '\0') {
            printf("Invalid value: %s for flag %s\n", value, flag);
            return;
        }

        if (strcmp(flag, "-a") == 0) {
            a = value_l;
        }
        else if (strcmp(flag, "-b") == 0) {
            b = value_l;
        }
        else {
            printf("Flag %s is not legit.\n", flag);
            return;
        }

        i = i + 2;
    }
    printf("A is %d, B is %d\n", a, b);

    return;
}

int main(int argc, char * argv[])
{
    // Check if the number of arguments are valid.
    if (argc == ARG_COUNT) {
        run_program(argv);
    }

    else{
        printf("The number of arguments should be equal to: %d\n",
               ARG_COUNT - 1);
    }

    return(0);
}
