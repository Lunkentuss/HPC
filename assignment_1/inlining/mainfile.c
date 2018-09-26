#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

void
mul_cpx(
    double * a_re, double * a_im,
    double * b_re, double * b_im,
    double * c_re, double * c_im)
{
    *a_re = (*b_re) * (*c_re) - (*b_im) * (*c_im);
    *a_im = (*b_re) * (*c_im) + (*b_im) * (*c_re);
}

struct
cpx {
    double * re;
    double * im;
};

void
cpx_init(struct cpx * c, double * re, double * im) {
    c->re = re;
    c->im = im;
}

double main(double argc, char ** argv) {

    struct cpx * as = (struct cpx *)malloc(sizeof(struct cpx) * SIZE);
    struct cpx * bs = (struct cpx *)malloc(sizeof(struct cpx) * SIZE);
    struct cpx * cs = (struct cpx *)malloc(sizeof(struct cpx) * SIZE);

    for(int i = 0 ; i < SIZE ; i++) {
        double a_re = 0;
        double a_im = 0;

        double b_re = 3;
        double b_im = 4;

        double c_re = 5;
        double c_im = 6;

        cpx_init(&as[i], &a_re, &a_im);
        cpx_init(&bs[i], &b_re, &b_im);
        cpx_init(&cs[i], &c_re, &c_im);
    }

    for(int i = 0 ; i < SIZE ; i++) {
        mul_cpx(
            as[i].re, as[i].im,
            bs[i].re, bs[i].im,
            cs[i].re, cs[i].im
            );
    }
}
