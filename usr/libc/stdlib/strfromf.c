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
    
    long long i = (long long) fp;
    lltoa(i, str, 10);

    int intDigits = 0;
    if(i == 0) intDigits = 1;
    else 
    {
        for(long long num = i; num != 0; num /= 10, intDigits++);
        if(i < 0) intDigits += 1;
    }

    str += intDigits;
    *(str++) = '.';

    float dec = fabsf((float) (fp - i));
    long long decl = 0;
    int low9 = 1;
    while(prec != 0 && dec > 0.0001f && (low9 = dec < 0.9999f))
    {
        dec *= 10;
        int digit = (int) dec;
        dec -= digit;
        decl = decl * 10 + digit;
        prec--;
    }
    if(!low9) decl += 1;
    if(prec > 0) while(prec-- > 0) decl *= 10;

    lltoa(decl, str, 10);
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
    
    long long i = (long long) fp;
    lltoa(i, str, 10);

    int intDigits = 0;
    if(i == 0) intDigits = 1;
    else 
    {
        for(long long num = i; num != 0; num /= 10, intDigits++);
        if(i < 0) intDigits += 1;
    }

    str += intDigits;
    *(str++) = '.';

    double dec = fabs(fp - i);
    long long decl = 0;
    int low9 = 1;
    while(prec != 0 && dec > 0.000001 && (low9 = dec < 0.999999))
    {
        dec *= 10;
        int digit = (int) dec;
        dec -= digit;
        decl = decl * 10 + digit;
        prec--;
    }
    if(!low9) decl += 1;
    if(prec > 0) while(prec-- > 0) decl *= 10;

    lltoa(decl, str, 10);
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
    
    long long i = (long long) fp;
    lltoa(i, str, 10);

    int intDigits = 0;
    if(i == 0) intDigits = 1;
    else 
    {
        for(long long num = i; num != 0; num /= 10, intDigits++);
        if(i < 0) intDigits += 1;
    }

    str += intDigits;
    *(str++) = '.';

    long double dec = fabsl(fp - i);
    long long decl = 0;
    int low9 = 1;
    while(prec != 0 && dec > 0.000001 && (low9 = dec < 0.999999))
    {
        dec *= 10;
        int digit = (int) dec;
        dec -= digit;
        decl = decl * 10 + digit;
        prec--;
    }
    if(!low9) decl += 1;
    if(prec > 0) while(prec-- > 0) decl *= 10;

    lltoa(decl, str, 10);
    return 0;
}