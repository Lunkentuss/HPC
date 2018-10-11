/* This header file holds code for doing exponential by squaring
   and some few case for specific exponential functions.*/

#ifndef H_EXP_BY_SQUARE
# define H_EXP_BY_SQUARE

# ifndef EXP_BY_SQUARE_TYPE
#  define EXP_BY_SQUARE_TYPE double complex
#  define EXP_BY_SQUARE_TYPE_UNITY 1
# endif

# ifndef POW2
#  define POW2(x) x * x
# endif

// Used to get a one argument function for the generic case,
// see pow_square_generic_fixed.
int FIXED_POWER;

EXP_BY_SQUARE_TYPE
pow_square_generic(EXP_BY_SQUARE_TYPE x, int n)
{
    EXP_BY_SQUARE_TYPE r = EXP_BY_SQUARE_TYPE_UNITY;
    while(n){
        if (n % 2 == 1){
            r *= x;
            n -= 1;
        }
        x *= x;
        n /= 2;
    }
    return(r);
}

EXP_BY_SQUARE_TYPE
pow_square_generic_fixed(const EXP_BY_SQUARE_TYPE x)
{
    return(pow_square_generic(x, FIXED_POWER));
}

EXP_BY_SQUARE_TYPE
pow_square_1(const EXP_BY_SQUARE_TYPE x)
{
    return x; 
}

EXP_BY_SQUARE_TYPE
pow_square_2(const EXP_BY_SQUARE_TYPE x)
{
    return POW2(x); 
}

EXP_BY_SQUARE_TYPE
pow_square_4(const EXP_BY_SQUARE_TYPE x)
{
    EXP_BY_SQUARE_TYPE x_r = POW2(x);
    x_r = POW2(x_r);
    return(x_r);
}

EXP_BY_SQUARE_TYPE
pow_square_6(const EXP_BY_SQUARE_TYPE x)
{
    EXP_BY_SQUARE_TYPE x_square = POW2(x);
    EXP_BY_SQUARE_TYPE x_triple = x_square * x;
    return(POW2(x_triple));
}

#endif
