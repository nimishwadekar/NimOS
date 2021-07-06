#pragma once

#define LOGGING 1

#ifdef LOGGING

// Formatted serial log output.
void Logf(const char *format, ...);

#endif