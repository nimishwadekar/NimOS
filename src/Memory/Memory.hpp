#pragma once

#include <stdint.h>

void memset(void *ptr, uint8_t value, uint64_t bytes);

int memcmp(const void *a, const void *b, uint64_t bytes);