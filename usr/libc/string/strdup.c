#include <stdlib.h>
#include <string.h>

char *strdup(const char *s)
{
	char *ptr = malloc(strlen(s) + 1);
    if(!ptr) return NULL;
    return strcpy(ptr, s);
}