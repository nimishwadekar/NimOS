#pragma once

#include <stdarg.h>

// Unsigned long integer to string.
char *ultoa(const unsigned long n, int radix, char *buffer);

// Unsigned long integer to string, length characters long.
char *ultona(const unsigned long n, int radix, int length, char *buffer);

// Formats the string.
char *FormatString(char *buffer, const char *format, va_list args);

// Length of string.
int strlen(const char *string);

// Copy string.
char *strcpy(const char *src, char *dst);

int strcmp(const char *a, const char *b);

// Replaces escape sequences with their literalized counterparts.
char *LiteralizeString(const char *string, char *buffer);

// String tokenizer.
char *strtok(char *str, const char *delim);