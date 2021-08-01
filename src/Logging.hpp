#pragma once

#define LOGGING 1

#ifdef LOGGING

// Formatted serial log output.
void logf(const char *format, ...);

#endif