#include <math.h>
#include "util.hpp"

int signnum(int value)
{
    return (0 < value) - (value < 0);
}

int mod(int a, int base)
{
    return ((a % base) + base) % base;
}

int find_distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}