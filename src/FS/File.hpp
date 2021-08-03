#pragma once
#include <stdint.h>

struct FILE
{
    char Name[64];
    uint32_t Flags;
    uint8_t Device; // Volume, 'A' to 'Z'.
    uint64_t Length;
    uint64_t CurrentCluster;
    uint64_t Position; // Offset in current cluster.
};