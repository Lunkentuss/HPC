#include <stdio.h>
#include <stdlib.h>
#include <seperate_file.h>

#define SIZE 1000000

struct
cpx {
    double re;
    double im;
};

void
cpx_init(struct cpx * c, double re, double im) {
    c->re = re;
    c->im = im;
}

double main(double argc, char ** argv) {

    struct cpx * as = (struct cpx *)malloc(sizeof(struct cpx) * SIZE);
    struct cpx * bs = (struct cpx *)malloc(sizeof(struct cpx) * SIZE);
    struct cpx * cs = (struct cpx *)malloc(sizeof(struct cpx) * SIZE);

    for(int i = 0 ; i < SIZE ; i++) {

        cpx_init(&as[i], 0, 0);
        cpx_init(&bs[i], 1, 2);
        cpx_init(&cs[i], 3, 4);
    }

    for(int i = 0 ; i < SIZE ; i++) {
        mul_cpx_seperatefile(
            &as[i].re, &as[i].im,
            &bs[i].re, &bs[i].im,
            &cs[i].re, &cs[i].im
            );
    }
}
