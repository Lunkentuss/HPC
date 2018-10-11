#ifndef H_EXP_BY_SQUARE
# define H_EXP_BY_SQUARE

# ifndef SQUARE_TYPE
#  define SQUARE_TYPE double complex
# endif

# ifndef POW2
#  define POW2(x) x * x
# endif

inline SQUARE_TYPE
pow_square_generic(SQUARE_TYPE x, int n)
{
    SQUARE_TYPE r = 1;
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

inline SQUARE_TYPE
pow_square_generic_D(const SQUARE_TYPE x)
{
    return(pow_square_generic(x, D));
}

inline SQUARE_TYPE
pow_square_2(const SQUARE_TYPE x)
{
    return POW2(x); 
}

inline SQUARE_TYPE
pow_square_3(const SQUARE_TYPE x)
{
    return(x * x * x);
}

inline SQUARE_TYPE
pow_square_5(const SQUARE_TYPE x)
{
    SQUARE_TYPE x_square = POW2(x);
    SQUARE_TYPE x_quad = POW2(x_square);
    return(x_quad * x);
}

inline SQUARE_TYPE
pow_square_7(const SQUARE_TYPE x)
{
    SQUARE_TYPE x_square = POW2(x);
    SQUARE_TYPE x_quad = POW2(x_square);
    return(x_quad * x_square * x);
}

#endif
