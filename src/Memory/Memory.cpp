#include "Memory.hpp"

void memset(void *ptr, uint8_t value, uint64_t bytes)
{
    uint8_t *bytePtr = (uint8_t*) ptr;
    for(uint64_t i = 0; i < bytes; i++) bytePtr[i] = value;
}

int memcmp(const void *a, const void *b, uint64_t bytes)
{
    const uint8_t *aptr = (const uint8_t*) a, *bptr = (const uint8_t*) b;
    for(uint64_t i = 0; i < bytes; i++) if(aptr[i] != bptr[i]) return aptr[i] - bptr[i];
    return 0;
}