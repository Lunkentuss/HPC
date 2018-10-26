#ifndef H_POINT
# define H_POINT

struct cell_point{
    int x;
    int y;
    int z;
};

void
print_point(const struct cell_point * p)
{
    printf("x: %d, y: %d, z: %d\n", p->x, p->y, p->z);
}

#endif
