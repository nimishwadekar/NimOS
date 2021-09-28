#include <stdlib.h>
#include <string.h>

char *itoa(int value, char *str, int base)
{
    if(value == 0)
    {
        str[0] = '0';
        str[1] = 0;
        return str;
    }

    int neg = 0;
    if(base == 10 && value < 0)
    {
        neg = 1;
        value = -value;
    }
    unsigned int n = (unsigned int) value;

    int i;
    for(i = 0; n > 0; n /= base, i++)
    {
        int r = n % base;
        str[i] = (r > 9) ? r - 10 + 'a' : r + '0';
    }
    if(neg) str[i++] = '-';
    str[i] = 0;

    strrev(str);
    return str;
}


char *ltoa(long int value, char *str, int base)
{
    if(value == 0)
    {
        str[0] = '0';
        str[1] = 0;
        return str;
    }

    int neg = 0;
    if(base == 10 && value < 0)
    {
        neg = 1;
        value = -value;
    }
    unsigned long int n = (unsigned long int) value;

    int i;
    for(i = 0; n > 0; n /= base, i++)
    {
        int r = n % base;
        str[i] = (r > 9) ? r - 10 + 'a' : r + '0';
    }
    if(neg) str[i++] = '-';
    str[i] = 0;

    strrev(str);
    return str;
}


char *lltoa(long long int value, char *str, int base)
{
    if(value == 0)
    {
        str[0] = '0';
        str[1] = 0;
        return str;
    }

    int neg = 0;
    if(base == 10 && value < 0)
    {
        neg = 1;
        value = -value;
    }
    unsigned long long int n = (unsigned long long int) value;

    int i;
    for(i = 0; n > 0; n /= base, i++)
    {
        int r = n % base;
        str[i] = (r > 9) ? r - 10 + 'a' : r + '0';
    }
    if(neg) str[i++] = '-';
    str[i] = 0;

    strrev(str);
    return str;
}