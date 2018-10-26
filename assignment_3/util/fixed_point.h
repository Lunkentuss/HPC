#ifndef H_FIXED_POINT
# define H_FIXED_POINT

#define FIXED_FACT 1000.0
#define FIXED_MULTIPLY(x, y) (int)((x) * (y) / FIXED_FACT)
#define INT_TO_FIXED(x) x * FIXED_FACT
#define FIXED_TO_INT(x) (int)(x / FIXED_FACT)
#define FLOAT_TO_FIXED(x) (int)(x * FIXED_FACT)
#define FIXED_TO_FLOAT(x) (float)((x) / FIXED_FACT)

#endif
