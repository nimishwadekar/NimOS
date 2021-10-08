#pragma once

#include <stdint.h>
#include <Bitmap.hpp>
#include <Memory/MemoryMap.hpp>

class PageFrameAllocator
{
    public:
    Bitmap PageFrameBitmap; // The bitmap that keeps track of free page frames.
    uint64_t FreeMemory; // Available memory.
    uint64_t ReservedMemory; // Memory reserved by system and devices, and memory holes.
    uint64_t UsedMemory; // Memory in use by processes.

    void Initialize(MemoryMap memoryMap);
    // Returns the physical address of a free page frame, and marks it as used.
    void *RequestPageFrame(void);
    // Marks the page frame as free.
    void FreePageFrame(void *physicalAddress);
    void *RequestPageFrames(const uint64_t frames);
};

extern PageFrameAllocator FrameAllocator;