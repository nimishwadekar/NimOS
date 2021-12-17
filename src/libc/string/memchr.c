#include <string.h>

void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = (const unsigned char *) s;
    for(; n; n--, p++) if(*p == c) return (void*) p;
    return NULL;
}


void *memrchr(const void *s, int c, size_t n)
{
	const unsigned char *p = (const unsigned char *) s;
    p += n - 1;
    for(; n; n--, p--) if(*p == c) return (void*) p;
    return NULL;
}


void *rawmemchr(const void *s, int c)
{
	const unsigned char *p = (const unsigned char *) s;
    for(; ; p++) if(*p == c) return (void*) p;
    return NULL;
}