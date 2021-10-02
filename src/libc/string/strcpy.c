#include <string.h>

char *strcpy(char *dest, const char *src)
{
	char *d = dest;
    for( ; *src; d++, src++) *d = *src;
    *d = 0;
    return dest;
}