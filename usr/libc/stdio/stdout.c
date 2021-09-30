#include <stdio.h>
#include <sys/syscall.h>

int printf(const char *format, ...)
{
	
}


int sprintf(char *str, const char *format, ...)
{
	
}


int puts(const char *s)
{
	
}


int putchar(int c)
{
	_syscall_2(SYS_PRINTN, (long long) &c, 1);
    return c;
}