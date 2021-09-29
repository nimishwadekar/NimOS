#include <ctype.h>
#include <stdlib.h>

unsigned long int strtoul(const char *nptr, char **endptr, int base)
{
	while(isspace(*nptr)) nptr++;

	int neg = 0;
    if(*nptr == '+' || *nptr == '-')
    {
        if(*nptr == '-') neg = 1;
        nptr++;
    }

    if(base == 0)
    {
        if(nptr[0] == '0')
        {    
            if(nptr[1] == 'x' || nptr[1] == 'X')
            {
                base = 16;
                nptr += 2;
            }
            else
            {
                base = 8;
                nptr++;
            }
        }
        else base = 10;
    }
    else if(base == 16)
    {
        if(nptr[0] == '0' && (nptr[1] == 'x' || nptr[1] == 'X')) nptr += 2;
    }
    else if(base < 2 || base > 36)
    {
        if(endptr) *endptr = (char*) nptr;
        return -1;
    }

    unsigned long num = 0;
	for( ; *nptr != 0; nptr++)
    {
        int digit = *nptr;
        if(isdigit(digit)) digit = digit - '0';
        else if(isalpha(digit))
        {
            digit = toupper(digit) - 'A' + 10;
            if(digit >= base)
            {
                if(endptr) *endptr = (char*) nptr;
                return -1;
            }
        }
        else
        {
            if(endptr) *endptr = (char*) nptr;
            return -1;
        } 
        num = num * base + digit;
    }
    if(endptr) *endptr = (char*) nptr;
    if(neg) num = -num;
    return num;
}


unsigned long long int strtoull(const char *nptr, char **endptr, int base)
{
	while(isspace(*nptr)) nptr++;

	int neg = 0;
    if(*nptr == '+' || *nptr == '-')
    {
        if(*nptr == '-') neg = 1;
        nptr++;
    }

    if(base == 0)
    {
        if(nptr[0] == '0')
        {    
            if(nptr[1] == 'x' || nptr[1] == 'X')
            {
                base = 16;
                nptr += 2;
            }
            else
            {
                base = 8;
                nptr++;
            }
        }
        else base = 10;
    }
    else if(base == 16)
    {
        if(nptr[0] == '0' && (nptr[1] == 'x' || nptr[1] == 'X')) nptr += 2;
    }
    else if(base < 2 || base > 36)
    {
        if(endptr) *endptr = (char*) nptr;
        return -1;
    }

    unsigned long long num = 0;
	for( ; *nptr != 0; nptr++)
    {
        int digit = *nptr;
        if(isdigit(digit)) digit = digit - '0';
        else if(isalpha(digit))
        {
            digit = toupper(digit) - 'A' + 10;
            if(digit >= base)
            {
                if(endptr) *endptr = (char*) nptr;
                return -1;
            }
        }
        else
        {
            if(endptr) *endptr = (char*) nptr;
            return -1;
        } 
        num = num * base + digit;
    }
    if(endptr) *endptr = (char*) nptr;
    if(neg) num = -num;
    return num;
}