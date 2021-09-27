#include <stdio.h>

extern void print(const char *str);

void printf(const char *format)
{
    print(format);
}