#include <string.h>

static int isdelim(int c, const char *delim)
{
    for( ; *delim; delim++) if(*delim == c) return 1;
    return 0;
}

static char *nextdelim(char *s, const char *delim)
{
    for(; *s && !isdelim(*s, delim); s++);
    return s; 
}

static char *nextnondelim(char *s, const char *delim)
{
    for(; *s && isdelim(*s, delim); s++);
    return s; 
}

char *strtok(char *str, const char *delim)
{
    static char *s = NULL;
    if(str) 
    {
        s = str;
        s = nextnondelim(s, delim);
    }
    if(!s) return NULL;

    if(!*s) return NULL;
    char *tok = s;
    char *del = nextdelim(s, delim);
    s = nextnondelim(del, delim);
    *del = 0;
    return tok;
}