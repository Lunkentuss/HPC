#ifndef H_FIXED_POINT
# define H_FIXED_POINT

#define DECI_FACT 1000
#define FIXED_POS 6
#define FIXED_MULTIPLY(x, y) ((x) * (y) >> FIXED_POS)
#define INT_TO_FIXED(x) ((x) << FIXED_POS)
#define FIXED_TO_INT(x) ((x) >> FIXED_POS)
#define FIXED_TO_FLOAT(x) (float)((x) >> FIXED_FACT)

static inline int
fixdeci_to_fixbin(const int deci, int sign)
{
}

#endif
