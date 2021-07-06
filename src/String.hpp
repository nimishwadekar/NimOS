#pragma once

#include <stdarg.h>

// Unsigned long integer to string.
char *ultoa(const unsigned long n, int radix, char *buffer);

// Unsigned long integer to hex string, 64 bits long.
char *ultoha(const unsigned long n, char *buffer);

// Formats the string.
char *FormatString(char *buffer, const char *format, va_list args);

// Length of string.
int strlen(const char *string);

// Copy string.
char *strcpy(const char *src, char *dst);

// Replaces escape sequences with their literalized counterparts.
char *LiteralizeString(const char *string, char *buffer);