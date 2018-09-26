#include <stdio.h>
#include <stdlib.h>

#define SIZE 30000

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
        mul_cpx(
            &as[i].re, &as[i].im,
            &bs[i].re, &bs[i].im,
            &cs[i].re, &cs[i].im
            );
    }
    printf("Re: %lf\n", as[0].re);
    printf("Im: %lf\n", as[0].im);
}
