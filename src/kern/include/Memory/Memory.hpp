#pragma once

#include <stdint.h>

void memset(void *ptr, uint8_t value, uint64_t bytes);

void memset32(void *ptr, uint32_t value, uint64_t dwords);

int memcmp(const void *a, const void *b, uint64_t bytes);

void memcpy(const void *from, void *to, uint64_t bytes);

inline uint64_t GetFullAddress(const uint32_t lower, const uint32_t upper)
{
    return lower | ((uint64_t) upper << 32);
}