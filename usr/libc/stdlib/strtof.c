#include <ctype.h>
#include <stdlib.h>

float strtof(const char *nptr, char **endptr)
{
	while(isspace(*nptr)) nptr++;

	long long int integer;
    float fraction;
    size_t point = 0;
    char *ptr = (char*) nptr;

    while(ptr[point] != 0 && ptr[point] != '.') point++;
    if(ptr[point] == 0) return (float) strtoll(nptr, endptr, 10);

    ptr[point] = 0;
    integer = strtoll(nptr, endptr, 10);
    if(endptr && **endptr != 0) return (float) integer;

    ptr[point] = '.';
    fraction = (float) strtoll(nptr + point + 1, endptr, 10);
    if(endptr && **endptr != 0) return (float) fraction;
    
    size_t declen = *endptr - (nptr + point + 1);
    while(declen--) fraction /= 10;
    return integer + fraction;
}


double strtod(const char *nptr, char **endptr)
{
    while(isspace(*nptr)) nptr++;

	long long int integer;
    double fraction;
    size_t point = 0;
    char *ptr = (char*) nptr;

    while(ptr[point] != 0 && ptr[point] != '.') point++;
    if(ptr[point] == 0) return (double) strtoll(nptr, endptr, 10);

    ptr[point] = 0;
    integer = strtoll(nptr, endptr, 10);
    if(endptr && **endptr != 0) return (double) integer;

    ptr[point] = '.';
    fraction = (double) strtoll(nptr + point + 1, endptr, 10);
    if(endptr && **endptr != 0) return (double) fraction;
    
    size_t declen = *endptr - (nptr + point + 1);
    while(declen--) fraction /= 10;
    return integer + fraction;
}


long double strtold(const char *nptr, char **endptr)
{
	while(isspace(*nptr)) nptr++;

	long long int integer;
    long double fraction;
    size_t point = 0;
    char *ptr = (char*) nptr;

    while(ptr[point] != 0 && ptr[point] != '.') point++;
    if(ptr[point] == 0) return (long double) strtoll(nptr, endptr, 10);

    ptr[point] = 0;
    integer = strtoll(nptr, endptr, 10);
    if(endptr && **endptr != 0) return (long double) integer;

    ptr[point] = '.';
    fraction = (long double) strtoll(nptr + point + 1, endptr, 10);
    if(endptr && **endptr != 0) return (long double) fraction;
    
    size_t declen = *endptr - (nptr + point + 1);
    while(declen--) fraction /= 10;
    return integer + fraction;
}