#include <string.h>

char *strchr(const char *s, int c)
{
	for(; *s; s++) if(*s == c) return (char*) s;
    if(!c) return (char*) s;
    return NULL;
}


char *strrchr(const char *s, int c)
{
    const char *ss = s;
    s = rawmemchr(s, 0);
    if(!c) return (char*) s;
    if(s == ss) return NULL;
    s--;
	for(; s != ss; s--) if(*s == c) return (char*) s;
    return NULL;
}