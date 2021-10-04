#include <String.hpp>
#include <Utility.hpp>

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

char *ultona(const unsigned long n, int radix, int length, char *buffer)
{
    if(n == 0)
    {
        for(int i = 0; i < length; i++) buffer[i] = '0';
        buffer[length] = 0;
        return buffer;
    }

    unsigned long number = n;
    buffer[length] = 0;
    for(int i = length - 1; i >= 0; i--, number /= radix)
    {
        char digit = number % radix;
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
        int digits = -1;
        if(isDigit(format[fIndex])) 
        {
            digits = format[fIndex] - '0';
            fIndex += 1;
        }
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
                if(digits == -1)
                {
                    bIndex += strlen(ultoa(u, 16, buffer + bIndex));
                }
                else
                {
                    ultona(u, 16, digits, buffer + bIndex);
                    bIndex += digits;
                }
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

int strcmp(const char *a, const char *b)
{
    int i = 0;
    for( ; a[i] && b[i]; i++)
    {
        if(a[i] != b[i]) break;
    }
    return a[i] - b[i];
}

char *LiteralizeString(const char *string, char *buffer)
{
    for(int i = 0, bi = 0; string[i] != 0; i++, bi++)
    {
        switch(string[i])
        {
            case '\n':
                buffer[bi] = '\\';
                bi += 1;
                buffer[bi] = 'n';
                break;
            
            case '\t':
                buffer[bi] = '\\';
                bi += 1;
                buffer[bi] = 't';
                break;

            default:
                buffer[bi] = string[i];
                break;
        }
    }
    return buffer;
}


// strtok()

static int isdelim(int c, const char *delim)
{
    for( ; *delim; delim++) if(*delim == c) return 1;
    return 0;
}

static char *nextdelim(char *s, const char *delim)
{
    for(; *s && !isdelim(*s, delim); s++);
    return s; 
}

static char *nextnondelim(char *s, const char *delim)
{
    for(; *s && isdelim(*s, delim); s++);
    return s; 
}

char *strtok(char *str, const char *delim)
{
    static char *s = nullptr;
    if(str) 
    {
        s = str;
        s = nextnondelim(s, delim);
    }
    if(!s) return nullptr;

    if(!*s) return nullptr;
    char *tok = s;
    char *del = nextdelim(s, delim);
    s = nextnondelim(del, delim);
    *del = 0;
    return tok;
}