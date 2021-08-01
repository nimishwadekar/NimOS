#include "Logging.hpp"

#ifdef LOGGING

#include <stddef.h>
#include "String.hpp"
#include "IO/Serial.hpp"

char LogFormattedStringBuffer[200];
void logf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    FormatString(LogFormattedStringBuffer, format, args);
    va_end(args);

    for(int i = 0; LogFormattedStringBuffer[i] != 0; i++)
    {
        SerialOut(SERIAL_COM1, LogFormattedStringBuffer[i]);
    }
}

#endif