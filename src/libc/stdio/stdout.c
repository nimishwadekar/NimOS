#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

static int _vaprintf(char *buffer, const char *format, va_list args, int stdoutput);

static char stdout_buffer[BUFSIZ + 64] = {};

int printf(const char *format, ...)
{
	va_list args;
    va_start(args, format);
    int ret = _vaprintf(stdout_buffer, format, args, 1);
    va_end(args);
    _syscall_1(SYS_PRINT, (long long) stdout_buffer);
    return ret;
}


int sprintf(char *str, const char *format, ...)
{
	va_list args;
    va_start(args, format);
    int ret = _vaprintf(str, format, args, 0);
    va_end(args);
    return ret;
}


int puts(const char *s)
{
	_syscall_1(SYS_PRINT, (long long) s);
    return strlen(s);
}


int putchar(int c)
{
	_syscall_2(SYS_PRINTN, (long long) &c, 1);
    return c;
}


int _vaprintf(char *buffer, const char *format, va_list args, int stdoutput)
{
    int fIndex = 0, bIndex = 0, formatBegin, formatNumEnd;
    int lmod = 0, llmod = 0;
    while(format[fIndex] != 0)
    {
        if(stdoutput && bIndex >= BUFSIZ) break;
        
        if(format[fIndex] != '%')
        {
            buffer[bIndex] = format[fIndex];
            fIndex += 1;
            bIndex += 1;
            continue;
        }
        
        formatBegin = fIndex++;
        if(format[fIndex] == '.') fIndex++;
        while(isdigit(format[fIndex])) fIndex++;
        formatNumEnd = fIndex;

        if(format[fIndex] == 'l')
        {
            if(format[fIndex + 1] == 'l')
            {
                llmod = 1;
                fIndex += 2;
            }
            else
            {
                lmod = 1;
                fIndex++;
            }
        }

        switch(format[fIndex])
        {
            case '%':
            {
                buffer[bIndex++] = '%';
                fIndex++;
            }
            break;

            case 'd':
            {
                int padding = 0;
                if(llmod)
                {
                    long long d = va_arg(args, long long);
                    size_t len = strlen(lltoa(d, buffer + bIndex, 10));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        int neg = d < 0;
                        if(neg) padding++;
                        memmove(buffer + bIndex + padding + (neg ? 1 : 0), buffer + bIndex + (neg ? 1 : 0), len - (neg ? 1 : 0));
                        memset(buffer + bIndex + (neg ? 1 : 0), '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                else if(lmod)
                {
                    long d = va_arg(args, long);
                    size_t len = strlen(ltoa(d, buffer + bIndex, 10));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        int neg = d < 0;
                        if(neg) padding++;
                        memmove(buffer + bIndex + padding + (neg ? 1 : 0), buffer + bIndex + (neg ? 1 : 0), len - (neg ? 1 : 0));
                        memset(buffer + bIndex + (neg ? 1 : 0), '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                else
                {
                    int d = va_arg(args, int);
                    size_t len = strlen(itoa(d, buffer + bIndex, 10));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        int neg = d < 0;
                        if(neg) padding++;
                        memmove(buffer + bIndex + padding + (neg ? 1 : 0), buffer + bIndex + (neg ? 1 : 0), len - (neg ? 1 : 0));
                        memset(buffer + bIndex + (neg ? 1 : 0), '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                fIndex++;
            }
            break;

            case 'u':
            {
                int padding = 0;
                if(llmod)
                {
                    unsigned long long d = va_arg(args, unsigned  long long);
                    size_t len = strlen(ulltoa(d, buffer + bIndex, 10));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                else if(lmod)
                {
                    unsigned long d = va_arg(args, unsigned long);
                    size_t len = strlen(ultoa(d, buffer + bIndex, 10));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                else
                {
                    unsigned int d = va_arg(args, unsigned int);
                    size_t len = strlen(utoa(d, buffer + bIndex, 10));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                fIndex++;
            }
            break;

            case 'o':
            {
                int padding = 0;
                if(llmod)
                {
                    unsigned long long d = va_arg(args, unsigned  long long);
                    size_t len = strlen(ulltoa(d, buffer + bIndex, 8));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                else if(lmod)
                {
                    unsigned long d = va_arg(args, unsigned long);
                    size_t len = strlen(ultoa(d, buffer + bIndex, 8));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                else
                {
                    unsigned int d = va_arg(args, unsigned int);
                    size_t len = strlen(utoa(d, buffer + bIndex, 8));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                fIndex++;
            }
            break;

            case 'X':
            case 'x':
            {
                int padding = 0;
                if(llmod)
                {
                    unsigned long long d = va_arg(args, unsigned  long long);
                    size_t len = strlen(ulltoa(d, buffer + bIndex, 16));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                else if(lmod)
                {
                    unsigned long d = va_arg(args, unsigned long);
                    size_t len = strlen(ultoa(d, buffer + bIndex, 16));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                else
                {
                    unsigned int d = va_arg(args, unsigned int);
                    size_t len = strlen(utoa(d, buffer + bIndex, 16));
                    if(formatNumEnd != formatBegin + 1)
                        padding = atoi(format + formatBegin + 1);
                    if(len < padding)
                    {
                        padding -= len;
                        memmove(buffer + bIndex + padding, buffer + bIndex, len);
                        memset(buffer + bIndex, '0', padding);
                        bIndex += padding;
                    }
                    bIndex += len;
                }
                fIndex++;
            }
            break;
            
            case 'F':
            case 'f':
            case 'G':
            case 'g':
            {
                double d = va_arg(args, double);
                strfromd(buffer + bIndex, 0, format + formatBegin, d);
                bIndex += strlen(buffer + bIndex);
                fIndex++;
            }
            break;

            case 'c':
            {
                char c = (char) va_arg(args, int);
                buffer[bIndex++] = c;
                fIndex++;
            }
            break;

            case 's':
            {
                char *s = va_arg(args, char*);
                bIndex += strlen(strcpy(buffer + bIndex, s));
                fIndex++;
            }
            break;

            case 'p':
            {
                unsigned long long d = (unsigned long long) va_arg(args, void*);
                bIndex += strlen(ulltoa(d, buffer + bIndex, 16));
                fIndex++;
            }
            break;

            case 'n':
            {
                int *d = va_arg(args, int*);
                *d = bIndex;
                fIndex++;
            }
            break;
        }
        lmod = 0;
        llmod = 0;
    }
    buffer[bIndex] = 0;
    return bIndex;
}