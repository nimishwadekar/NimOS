#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	const char *s = (const char*) src;
    char *d = (char*) dest;
    for( ; n; n--, s++, d++) *d = *s;
    return dest;
}


void *memccpy(void *dest, const void *src, int c, size_t n)
{
	const char *s = (const char*) src;
    char *d = (char*) dest;
    int notfound = 1;
    for( ; n && (notfound = *s != c); n--, s++, d++) *d = *s;
    if(notfound) return NULL;
    return d;
}


void *memmove(void *dest, const void *src, size_t n)
{
	const char *s = (const char*) src;
    char *d = (char*) dest;
    if(s == d) return dest;
    if(s > d) return memcpy(dest, src, n);

    d = d + n - 1;
    s = s + n - 1;
    for( ; n; n--, s--, d--) *d = *s;
    return dest;
}