#include <math.h>

double sqrt(double arg)
{
    if(arg < 0) return -1.0;
    if(arg == 0.0 || arg == 1.0) return arg;

    double lo = 0, hi = arg, mid, mid2, ans = -1;
    while(hi - lo > 0.000001)
    {
        mid = (lo + hi) / 2;
        mid2 = mid * mid;
        if(mid2 == arg) return mid;
        else if(mid2 < arg) lo = mid;
        else hi = mid;
    }
    return mid;
 }