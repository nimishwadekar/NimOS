#include <string.h>

char *strrev(char *str)
{
    size_t len = strlen(str);
    char *rstr = str + len - 1;
    char *retVal = str;
    for( ; str < rstr; str++, rstr--)
    {
        char tmp = *str;
        *str = *rstr;
        *rstr = tmp;
    }
    return retVal;
}