#include "Utility.hpp"

bool isAlpha(const char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isDigit(const char c)
{
    return c >= '0' && c <= '9';
}

bool isAlnum(const char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

bool isUpper(const char c)
{
    return c >= 'A' && c <= 'Z';
}

bool isLower(const char c)
{
    return c >= 'a' && c <= 'z';
}

bool isSpace(const char c)
{
    return c == ' ' || c == '\n' || c == '\t';
}

bool isSpecial(const char c)
{
    return !(isAlnum(c) || isSpace(c));
}