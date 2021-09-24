#include <Bitmap.hpp>

void Bitmap::Initialize(void *buffer, uint64_t bufferSize, bool fill)
{
    Buffer = (uint8_t*) buffer;
    BufferSize = bufferSize;

    uint8_t value = 0;
    if(fill) value = 0xFF;
    for(uint64_t i = 0; i < bufferSize; i++) Buffer[i] = value;
}

bool Bitmap::Get(const uint64_t index)
{
    uint64_t byteIndex = index >> 3; // index / 8
    uint8_t bit = index & 0b111; // index % 8
    return Buffer[byteIndex] & (0b10000000 >> bit);
}

void Bitmap::Set(const uint64_t index)
{
    uint64_t byteIndex = index >> 3; // index / 8
    uint8_t bit = index & 0b111; // index % 8
    Buffer[byteIndex] |= 0b10000000 >> bit;
}

void Bitmap::Clear(const uint64_t index)
{
    uint64_t byteIndex = index >> 3; // index / 8
    uint8_t bit = index & 0b111; // index % 8
    Buffer[byteIndex] &= ~(0b10000000 >> bit);
}