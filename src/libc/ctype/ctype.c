#include <ctype.h>

int isalnum(int c)
{
    return (isalpha(c) || isdigit(c));
}


int isalpha(int c)
{
    return (isupper(c) || islower(c));
}


int iscntrl(int c)
{
    return c < 32;
}


int isdigit(int c)
{
    return c >= '0' && c <= '9';
}


int isgraph(int c)
{
    return c > 32;
}


int islower(int c)
{
    return c >= 'a' && c <= 'z';
}


int isprint(int c)
{
    return c >= 32;
}


int ispunct(int c)
{
    return c > 32 && !isalnum(c);
}


int isspace(int c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\v' || c == '\r';
}


int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}


int isxdigit(int c)
{
    return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}


int isascii(int c)
{
    return c >= 0 && c < 128;
}


int isblank(int c)
{
    return c == ' ' || c == '\t' || c == '\v';
}


int tolower(int c)
{
    return islower(c) ? c : c + 32;
}


int toupper(int c)
{
    return isupper(c) ? c : c - 32;
}


int toascii(int c)
{
    return c & 0x7F;
}