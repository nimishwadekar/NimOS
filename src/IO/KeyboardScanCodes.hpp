#pragma once

#include <stdint.h>

namespace QWERTYKeyboard
{
    extern const uint8_t ALTERNATE_PREFIX;
    extern const uint8_t RELEASE_OFFSET;
    extern const char ScanCodeTable[];
    char TranslateScanCode(const uint8_t scanCode);
}