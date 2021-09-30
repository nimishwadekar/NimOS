#include <string.h>

char *strcat(char *dest, const char *src)
{
	void *append = rawmemchr(dest, 0);
    strcpy(append, src);
    return dest;
}