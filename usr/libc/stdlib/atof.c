#include <stdlib.h>
#include <string.h>

double atof(const char *nptr)
{
	long long int integer;
    double fraction;
    size_t point = 0;
    char *ptr = (char*) nptr;

    while(ptr[point] != 0 && ptr[point] != '.') point++;
    if(ptr[point] == 0) return (double) atoll(nptr);

    ptr[point] = 0;
    integer = atoll(ptr);
    ptr[point] = '.';
    fraction = (double) atoll(ptr + point + 1);
    size_t declen = strlen(ptr + point + 1);
    while(declen--) fraction /= 10;
    return integer + fraction;
}