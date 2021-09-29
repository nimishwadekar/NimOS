#include <math.h>

double fabs(double j)
{
    return (j >= 0) ? j : -j;
}


double fabsf(float j)
{
    return (j >= 0) ? j : -j;
}


double fabsl(long double j)
{
    return (j >= 0) ? j : -j;
}