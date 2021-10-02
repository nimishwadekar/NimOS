#pragma once

#include <stdint.h>

class Bitmap
{
    public:
    uint8_t *Buffer; // The buffer to store the bits.
    uint64_t BufferSize; // Size of the buffer in bytes.

    void Initialize(void *buffer, uint64_t bufferSize, bool fill);
    bool Get(const uint64_t index);
    void Set(const uint64_t index);
    void Clear(const uint64_t index);
};