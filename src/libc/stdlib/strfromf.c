#include <ctype.h>
#include <math.h>
#include <stdlib.h>

int strfromf(char *restrict str, size_t n, const char *restrict format, float fp)
{
	if(*(format++) != '%') return -1;
    
    int prec = 0;
    if(*format == '.')
    {
        format++;
        while(isdigit(*format)) prec = prec * 10 + *(format++) - '0';
    }
    if(prec == 0) prec = -1;
    int formatl = tolower(*format);
    if(formatl != 'a' && formatl != 'e' && formatl != 'f' && formatl != 'g') return -1;
    
    long long i = (long long) (fp + 0.000001);

    int intDigits = 0;
    if(i == 0) intDigits = 1;
    else 
    {
        for(long long num = i; num != 0; num /= 10, intDigits++);
        if(i < 0) intDigits += 1;
    }

    float dec = fabsf(fp - i);
    unsigned long long decl = (unsigned long long) (dec * 1000000);
    if(decl == 999999)
    {
        i += 1;
        decl = 0;
    }

    lltoa(i, str, 10);
    str += intDigits;
    *(str++) = '.';

    if(decl == 0)
    {
        *str = '0';
        *(str + 1) = 0;
        return 0;
    }

    while(decl % 10 == 0) decl /= 10;

    ulltoa(decl, str, 10);
    return 0;
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
    if(prec == 0) prec = -1;
    int formatl = tolower(*format);
    if(formatl != 'a' && formatl != 'e' && formatl != 'f' && formatl != 'g') return -1;
    
    long long i = (long long) (fp + 0.000001);

    int intDigits = 0;
    if(i == 0) intDigits = 1;
    else 
    {
        for(long long num = i; num != 0; num /= 10, intDigits++);
        if(i < 0) intDigits += 1;
    }

    double dec = fabs(fp - i);
    unsigned long long decl = (unsigned long long) (dec * 1000000);
    if(decl == 999999)
    {
        i += 1;
        decl = 0;
    }

    lltoa(i, str, 10);
    str += intDigits;
    *(str++) = '.';

    if(decl == 0)
    {
        *str = '0';
        *(str + 1) = 0;
        return 0;
    }

    while(decl % 10 == 0) decl /= 10;

    ulltoa(decl, str, 10);
    return 0;
}


int strfroml(char *restrict str, size_t n, const char *restrict format, long double fp)
{
	if(*(format++) != '%') return -1;
    
    int prec = 0;
    if(*format == '.')
    {
        format++;
        while(isdigit(*format)) prec = prec * 10 + *(format++) - '0';
    }
    if(prec == 0) prec = -1;
    int formatl = tolower(*format);
    if(formatl != 'a' && formatl != 'e' && formatl != 'f' && formatl != 'g') return -1;
    
    long long i = (long long) (fp + 0.000001);

    int intDigits = 0;
    if(i == 0) intDigits = 1;
    else 
    {
        for(long long num = i; num != 0; num /= 10, intDigits++);
        if(i < 0) intDigits += 1;
    }

    long double dec = fabsl(fp - i);
    unsigned long long decl = (unsigned long long) (dec * 1000000);
    if(decl == 999999)
    {
        i += 1;
        decl = 0;
    }

    lltoa(i, str, 10);
    str += intDigits;
    *(str++) = '.';

    if(decl == 0)
    {
        *str = '0';
        *(str + 1) = 0;
        return 0;
    }

    while(decl % 10 == 0) decl /= 10;

    ulltoa(decl, str, 10);
    return 0;
}