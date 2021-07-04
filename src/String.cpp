#include "String.hpp"

char *ultoa(const unsigned long n, int radix, char *buffer)
{
    if(n == 0)
    {
        buffer[0] = '0';
        buffer[1] = 0;
        return buffer;
    }

    unsigned long number = n;
    int digits = 0;
    for( ; number > 0; number /= radix, digits++);
    
    number = n;
    buffer[digits] = 0;
    for(int i = digits - 1; i >= 0; i--, number /= radix)
    {
        char digit = number % radix;
        if(digit > 9) digit = digit - 10 + 'A';
        else digit = digit + '0';
        buffer[i] = digit;
    }
    return buffer;
}

char *ultoha(const unsigned long n, char *buffer)
{
    unsigned long number = n;
    buffer[16] = 0;
    for(int i = 15; i >= 0; i--, number /= 16)
    {
        char digit = number % 16;
        if(digit > 9) digit = digit - 10 + 'A';
        else digit = digit + '0';
        buffer[i] = digit;
    }
    return buffer;
}

char *FormatString(char *buffer, const char *format, va_list args)
{
    int fIndex = 0, bIndex = 0;
    while(format[fIndex] != 0)
    {
        if(format[fIndex] != '%')
        {
            buffer[bIndex] = format[fIndex];
            fIndex += 1;
            bIndex += 1;
            continue;
        }

        fIndex += 1;
        switch (format[fIndex])
        {
            case 'b':
            {
                unsigned long u = va_arg(args, unsigned long);
                bIndex += strlen(ultoa(u, 2, buffer + bIndex));
                fIndex += 1;
                break;
            }

            case 'c':
            {
                char c = (char) va_arg(args, int);
                buffer[bIndex] = c;
                bIndex += 1;
                fIndex += 1;
                break;
            }

            case 'h':
            {
                unsigned long u = va_arg(args, unsigned long);
                bIndex += strlen(ultoha(u, buffer + bIndex));
                fIndex += 1;
                break;
            }

            case 's':
            {
                const char *s = va_arg(args, const char*);
                int len = strlen(s);
                strcpy(s, buffer + bIndex);
                bIndex += len;
                fIndex += 1;
                break;
            }

            case 'u':
            {
                unsigned long u = va_arg(args, unsigned long);
                bIndex += strlen(ultoa(u, 10, buffer + bIndex));
                fIndex += 1;
                break;
            }

            case 'x':
            {
                unsigned long u = va_arg(args, unsigned long);
                bIndex += strlen(ultoa(u, 16, buffer + bIndex));
                fIndex += 1;
                break;
            }
        }
    }
    buffer[bIndex] = 0;
    return buffer;
}

int strlen(const char *string)
{
    int length = 0;
    for( ; string[length] != 0; length++);
    return length;
}

char *strcpy(const char *src, char *dst)
{
    int i = 0;
    for( ; src[i] != 0; i++) dst[i] = src[i];
    dst[i] = 0;
    return dst;
}