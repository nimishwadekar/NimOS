#include <math.h>

double fabs(double j)
{
    return (j >= 0) ? j : -j;
}


float fabsf(float j)
{
    return (j >= 0) ? j : -j;
}


long double fabsl(long double j)
{
    return (j >= 0) ? j : -j;
}