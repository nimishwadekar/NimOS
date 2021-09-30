#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

int scanf(const char *format, ...)
{
	
}


int sscanf(const char *str, const char *format, ...)
{
	
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