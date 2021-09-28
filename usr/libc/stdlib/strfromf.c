#include <stdlib.h>
#include <ctype.h>

int strfromf(char *restrict str, size_t n, const char *restrict format, float fp)
{
	
}


int strfromd(char *restrict str, size_t n, const char *restrict format, double fp)
{
	if(*(format++) != '%') return -1;
    
    int prec = 0;
    if(*format == '.')
    {
        format++;
        while(isdigit(*format)) prec = prec * 10 + *(format++) - '0';
    }
    int formatl = tolower(*format);
    if(formatl != 'a' && formatl != 'e' && formatl != 'f' && formatl != 'g') return -1;
    
    long long i = (long long) fp;
    lltoa(i, str, 10);
}


int strfroml(char *restrict str, size_t n, const char *restrict format, long double fp)
{
	
}