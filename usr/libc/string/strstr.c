#include <string.h>

char *strstr(const char *haystack, const char *needle)
{
	size_t len = strlen(haystack);
    size_t sublen = strlen(needle);
    if(sublen > len) return NULL;

    for(size_t i = 0; i <= len - sublen; i++) 
        if(memcmp(haystack + i, needle, sublen) == 0) return (char*) haystack + i;
    return NULL;
}