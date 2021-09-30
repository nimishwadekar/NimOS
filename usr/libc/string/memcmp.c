#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n)
{
	const char *p1 = (const char*) s1;
    const char *p2 = (const char*) s2;
    for(; n && *p1 == *p2; n--, p1++, p2++);
    if(!n) return 0;
    return *p1 - *p2;
}