#include "Memory.hpp"

void memset(void *ptr, uint8_t value, uint64_t bytes)
{
    uint8_t *bytePtr = (uint8_t*) ptr;
    for(uint64_t i = 0; i < bytes; i++) bytePtr[i] = value;
}