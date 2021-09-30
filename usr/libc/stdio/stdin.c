#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

static int _vascanf(const char *buffer, const char *format, va_list args);

static char stdin_buffer[BUFSIZ + 64];

int scanf(const char *format, ...)
{
    char c;
    int i = 0;
    while(i < BUFSIZ && (c = getchar()) != '\n') stdin_buffer[i++] = c;
    stdin_buffer[i] = 0;

	va_list args;
    va_start(args, format);
    int ret = _vascanf(stdin_buffer, format, args);
    va_end(args);
    return ret;
}


int sscanf(const char *str, const char *format, ...)
{
	va_list args;
    va_start(args, format);
    int ret = _vascanf(str, format, args);
    va_end(args);
    return ret;
}


char *gets(char *s)
{
    size_t end = _syscall_1(SYS_INPUT, (int64_t) s);
    *(s + end) = 0;
    return s;
}


int getchar(void)
{
	char c;
    _syscall_2(SYS_INPUTN, (int64_t) &c, 1);
    return c;
}


static int _vascanf(const char *buffer, const char *format, va_list args)
{
    int fIndex = 0, bIndex = 0;
    int lmod = 0, llmod = 0;
    char *endptr = "1";
    int entries = 0;

    while(format[fIndex] && buffer[bIndex])
    {
        if(format[fIndex++] != '%') continue;
        //while(buffer[bIndex] && isspace(buffer[bIndex])) bIndex++;

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
                fIndex++;
            }
            break;

            case 'i':
            {
                if(llmod)
                {
                    long long *d = va_arg(args, long long*);
                    *d = strtoll(buffer + bIndex, &endptr, 0);
                    bIndex += endptr - (buffer + bIndex);
                }
                else if(lmod)
                {
                    long *d = va_arg(args, long*);
                    *d = strtol(buffer + bIndex, &endptr, 0);
                    bIndex += endptr - (buffer + bIndex);
                }
                else
                {
                    int *d = va_arg(args, int*);
                    *d = (int) strtol(buffer + bIndex, &endptr, 0);
                    bIndex += endptr - (buffer + bIndex);
                }
                fIndex++;
            }
            entries++;
            break;

            case 'd':
            {
                if(llmod)
                {
                    long long *d = va_arg(args, long long*);
                    *d = strtoll(buffer + bIndex, &endptr, 10);
                    bIndex += endptr - (buffer + bIndex);
                }
                else if(lmod)
                {
                    long *d = va_arg(args, long*);
                    *d = strtol(buffer + bIndex, &endptr, 10);
                    bIndex += endptr - (buffer + bIndex);
                }
                else
                {
                    int *d = va_arg(args, int*);
                    *d = (int) strtol(buffer + bIndex, &endptr, 10);
                    bIndex += endptr - (buffer + bIndex);
                }
                fIndex++;
            }
            entries++;
            break;

            case 'u':
            {
                if(llmod)
                {
                    unsigned long long *d = va_arg(args, unsigned long long*);
                    *d = strtoull(buffer + bIndex, &endptr, 10);
                    bIndex += endptr - (buffer + bIndex);
                }
                else if(lmod)
                {
                    unsigned long *d = va_arg(args, unsigned long*);
                    *d = strtoul(buffer + bIndex, &endptr, 10);
                    bIndex += endptr - (buffer + bIndex);
                }
                else
                {
                    unsigned int *d = va_arg(args, unsigned int*);
                    *d = (int) strtoul(buffer + bIndex, &endptr, 10);
                    bIndex += endptr - (buffer + bIndex);
                }
                fIndex++;
            }
            entries++;
            break;

            case 'o':
            {
                if(llmod)
                {
                    unsigned long long *d = va_arg(args, unsigned long long*);
                    *d = strtoull(buffer + bIndex, &endptr, 8);
                    bIndex += endptr - (buffer + bIndex);
                }
                else if(lmod)
                {
                    unsigned long *d = va_arg(args, unsigned long*);
                    *d = strtoul(buffer + bIndex, &endptr, 8);
                    bIndex += endptr - (buffer + bIndex);
                }
                else
                {
                    unsigned int *d = va_arg(args, unsigned int*);
                    *d = (int) strtoul(buffer + bIndex, &endptr, 8);
                    bIndex += endptr - (buffer + bIndex);
                }
                fIndex++;
            }
            entries++;
            break;

            case 'X':
            case 'x':
            {
                if(llmod)
                {
                    unsigned long long *d = va_arg(args, unsigned long long*);
                    *d = strtoull(buffer + bIndex, &endptr, 16);
                    bIndex += endptr - (buffer + bIndex);
                }
                else if(lmod)
                {
                    unsigned long *d = va_arg(args, unsigned long*);
                    *d = strtoul(buffer + bIndex, &endptr, 16);
                    bIndex += endptr - (buffer + bIndex);
                }
                else
                {
                    unsigned int *d = va_arg(args, unsigned int*);
                    *d = (int) strtoul(buffer + bIndex, &endptr, 16);
                    bIndex += endptr - (buffer + bIndex);
                }
                fIndex++;
            }
            entries++;
            break;
            
            case 'F':
            case 'f':
            case 'G':
            case 'g':
            {
                float *d = va_arg(args, float*);
                strtof(buffer + bIndex, &endptr);
                bIndex += endptr - (buffer + bIndex);
                fIndex++;
            }
            entries++;
            break;

            case 's':
            {
                char *s = va_arg(args, char*);
                int begin = bIndex;
                for( ; buffer[bIndex] && !isspace(buffer[bIndex]); bIndex++);
                memcpy(s, buffer + begin, bIndex - begin);
                s[bIndex - begin] = 0;
                fIndex++;
            }
            entries++;
            break;

            case 'p':
            {
                void** d = (void**) va_arg(args, void**);
                *d = (void*) strtoull(buffer + bIndex, &endptr, 16);
                bIndex += endptr - (buffer + bIndex);
                fIndex++;
            }
            entries++;
            break;

            case 'n':
            {
                int *d = va_arg(args, int*);
                *d = entries;
                fIndex++;
            }
            break;
        }
        lmod = 0;
        llmod = 0;

        if(!*endptr) break;
        if(!isspace(*endptr))
        {
            while(buffer[bIndex] && !isspace(buffer[bIndex])) bIndex++;
            endptr = "1";
            entries--;
        }
        while(buffer[bIndex] && isspace(buffer[bIndex])) bIndex++;
    }
    return entries;
}