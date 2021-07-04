#pragma once

#include <stdint.h>

class Bitmap
{
    public:
    uint8_t *Buffer; // The buffer to store the bits.
    uint64_t BufferLength; // Length of the buffer in bytes.
    uint64_t CurrentByte; // The first 0 bit.

    Bitmap(uint8_t *buffer, uint64_t bufferLength);
    bool Get(const uint64_t index);
    void Set(const uint64_t index, const bool value);
};