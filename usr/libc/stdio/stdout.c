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
    int fIndex = 0, bIndex = 0, formatBegin;
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
            case 'i':
            {
                if(llmod)
                {
                    long long d = va_arg(args, long long);
                    bIndex += strlen(lltoa(d, buffer + bIndex, 10));
                }
                else if(lmod)
                {
                    long d = va_arg(args, long);
                    bIndex += strlen(ltoa(d, buffer + bIndex, 10));
                }
                else
                {
                    int d = va_arg(args, int);
                    bIndex += strlen(itoa(d, buffer + bIndex, 10));
                }
                fIndex++;
            }
            break;

            case 'u':
            {
                if(llmod)
                {
                    unsigned long long d = va_arg(args, unsigned  long long);
                    bIndex += strlen(ulltoa(d, buffer + bIndex, 10));
                }
                else if(lmod)
                {
                    unsigned long d = va_arg(args, unsigned long);
                    bIndex += strlen(ultoa(d, buffer + bIndex, 10));
                }
                else
                {
                    unsigned int d = va_arg(args, unsigned int);
                    bIndex += strlen(utoa(d, buffer + bIndex, 10));
                }
                fIndex++;
            }
            break;

            case 'o':
            {
                if(llmod)
                {
                    unsigned long long d = va_arg(args, unsigned  long long);
                    bIndex += strlen(ulltoa(d, buffer + bIndex, 8));
                }
                else if(lmod)
                {
                    unsigned long d = va_arg(args, unsigned long);
                    bIndex += strlen(ultoa(d, buffer + bIndex, 8));
                }
                else
                {
                    unsigned int d = va_arg(args, unsigned int);
                    bIndex += strlen(utoa(d, buffer + bIndex, 8));
                }
                fIndex++;
            }
            break;

            case 'X':
            case 'x':
            {
                if(llmod)
                {
                    unsigned long long d = va_arg(args, unsigned  long long);
                    bIndex += strlen(ulltoa(d, buffer + bIndex, 16));
                }
                else if(lmod)
                {
                    unsigned long d = va_arg(args, unsigned long);
                    bIndex += strlen(ultoa(d, buffer + bIndex, 16));
                }
                else
                {
                    unsigned int d = va_arg(args, unsigned int);
                    bIndex += strlen(utoa(d, buffer + bIndex, 16));
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