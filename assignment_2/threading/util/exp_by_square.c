#ifndef H_EXP_BY_SQUARE
# define H_EXP_BY_SQUARE

# ifndef SQUARE_TYPE
#  define SQUARE_TYPE double complex
# endif

SQUARE_TYPE inline
pow_square_generic(const SQUARE_TYPE x)
{

}

SQUARE_TYPE inline
pow_square_2(const SQUARE_TYPE x)
{
    return x * x; 
}

SQUARE_TYPE inline
pow_square_3(const SQUARE_TYPE x)
{
    return x * x * x;
}

SQUARE_TYPE inline
pow_square_5(const SQUARE_TYPE x)
{
    SQUARE_TYPE x_r = x;
    x_r = x_r * x_r;
    x_r = x_r * x_r;
    x_r = x_r * x;
    return x_r;
}

SQUARE_TYPE inline
pow_square_7(const SQUARE_TYPE x)
{
    SQUARE_TYPE x_r = x;
    x_r = x_r * x;
    return x_r;
}

#endif
