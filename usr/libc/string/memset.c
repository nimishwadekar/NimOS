#include <string.h>

void *memset(void *s, int c, size_t n)
{
	unsigned char *ptr = (unsigned char*) s;
    for( ; n; n--, ptr++) *ptr = c;
    return s;
}