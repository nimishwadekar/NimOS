#pragma once

bool isAlpha(const char c);
bool isDigit(const char c);
bool isAlnum(const char c);
bool isUpper(const char c);
bool isLower(const char c);
bool isSpace(const char c);

// Not alpha, digit, space.
bool isSpecial(const char c);