#include <stdlib.h>

int atoi(const char *nptr)
{
    int neg = 0;
    if(*nptr == '+' || *nptr == '-')
    {
        if(*nptr == '-') neg = 1;
        nptr++;
    }

    int num = 0;
	for( ; *nptr != 0; nptr++) num = num * 10 + (*nptr - '0');
    if(neg) num = -num;
    return num;
}


long atol(const char *nptr)
{
    int neg = 0;
    if(*nptr == '+' || *nptr == '-')
    {
        if(*nptr == '-') neg = 1;
        nptr++;
    }

    long num = 0;
	for( ; *nptr != 0; nptr++) num = num * 10 + (*nptr - '0');
    if(neg) num = -num;
    return num;
}


long long atoll(const char *nptr)
{
	int neg = 0;
    if(*nptr == '+' || *nptr == '-')
    {
        if(*nptr == '-') neg = 1;
        nptr++;
    }

    long long num = 0;
	for( ; *nptr != 0; nptr++) num = num * 10 + (*nptr - '0');
    if(neg) num = -num;
    return num;
}