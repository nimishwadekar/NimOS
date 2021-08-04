#pragma once
#include <stdint.h>

#define FS_INVALID 0
#define FS_VALID 1
#define FS_FILE (1 << 1)
#define FS_DIRECTORY (1 << 2)
#define FS_WRITE (1 << 3)
#define FS_APPEND (1 << 4)

struct FILE
{
    char Name[64];
    uint32_t Flags;
    uint8_t Device; // Volume, 'A' to 'Z'.
    uint64_t Length;
    uint64_t CurrentCluster;
    uint64_t Position; // Offset in current cluster.
};