#include "Bitmap.hpp"

Bitmap::Bitmap(uint8_t *buffer, uint64_t bufferLength) :
    Buffer(buffer), BufferLength(bufferLength) { }

bool Bitmap::Get(const uint64_t index)
{
    uint64_t byteIndex = index >> 3; // index / 8
    uint8_t bit = index & 0b111; // index % 8
    return Buffer[byteIndex] & (0b10000000 >> bit);
}

void Bitmap::Set(const uint64_t index, const bool value)
{
    uint64_t byteIndex = index >> 3; // index / 8
    uint8_t bit = index & 0b111; // index % 8
    if(value) Buffer[byteIndex] |= 0b10000000 >> bit;
    else Buffer[byteIndex] &= ~(0b10000000 >> bit);
}