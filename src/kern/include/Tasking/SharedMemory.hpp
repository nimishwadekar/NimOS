#pragma once

#include <stdint.h>

#define SHM_ADDR_BASE   0x800000000
#define SHM_SEG_SIZE    0x1000
#define SHM_MAX_SEGS    256

class SharedMemoryManager
{
    uint8_t AttachedProcessCount[SHM_MAX_SEGS];
    void *AttachedPageFrames[SHM_MAX_SEGS];

    public:
    void Initialise();
    void *AttachProcess(uint8_t key);
    void DetachProcess(uint8_t key);
    void *GetShmAddress(uint8_t key);
};

extern SharedMemoryManager ShmManager;