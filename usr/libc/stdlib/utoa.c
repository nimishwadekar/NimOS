#include <stdlib.h>

static void reverse(char* a, char *b)
{
    for( ; a < b; a++, b--)
    {
        char tmp = *a;
        *a = *b;
        *b = tmp;
    }
}

char *utoa(unsigned int value, char *str, int base)
{
    if(value == 0)
    {
        str[0] = '0';
        str[1] = 0;
        return str;
    }

    unsigned int n = value;

    int i;
    for(i = 0; n > 0; n /= base, i++)
    {
        int r = n % base;
        str[i] = (r > 9) ? r - 10 + 'a' : r + '0';
    }
    str[i] = 0;

    reverse(str, str + i - 1);
    return str;
}


char *ultoa(unsigned long int value, char *str, int base)
{
    if(value == 0)
    {
        str[0] = '0';
        str[1] = 0;
        return str;
    }

    unsigned long int n = value;

    int i;
    for(i = 0; n > 0; n /= base, i++)
    {
        int r = n % base;
        str[i] = (r > 9) ? r - 10 + 'a' : r + '0';
    }
    str[i] = 0;

    reverse(str, str + i - 1);
    return str;
}


char *ulltoa(unsigned long long int value, char *str, int base)
{
    if(value == 0)
    {
        str[0] = '0';
        str[1] = 0;
        return str;
    }

    unsigned long long int n = value;

    int i;
    for(i = 0; n > 0; n /= base, i++)
    {
        int r = n % base;
        str[i] = (r > 9) ? r - 10 + 'a' : r + '0';
    }
    str[i] = 0;

    reverse(str, str + i - 1);
    return str;
}