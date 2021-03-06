#include <Memory/PageFrameAllocator.hpp>

// The page frame allocator.
PageFrameAllocator FrameAllocator;

// So that memory map can be read only once.
bool Initialized = false;
uint64_t FirstFreePageFrame = 0;

void PageFrameAllocator::Initialize(MemoryMap memoryMap)
{
    if(Initialized) return;

    Initialized = true;

    MemoryMapEntry *entries = memoryMap.Entries;
    uint64_t largestFreeMemoryChunk = 0, largestFreeMemoryChunkSize = 0;
    for(unsigned int i = 0; i < memoryMap.EntryCount; i++) // Get largest free chunk of memory to store bitmap in.
    {
        if(MemoryMapEntry_Type(entries[i]) == MMAP_FREE && MemoryMapEntry_Size(entries[i]) > largestFreeMemoryChunkSize)
        {
            largestFreeMemoryChunkSize = MemoryMapEntry_Size(entries[i]);
            largestFreeMemoryChunk = entries[i].Address;
        }
    }

    uint64_t bitmapBufferSize = (memoryMap.MemorySizeKB + 31) / (4 * 8);
    PageFrameBitmap.Initialize((void*) largestFreeMemoryChunk, bitmapBufferSize, true); // Initialise all page frames as used initially.

    // Mark the page frames currently free according to the memory map as free.
    FreeMemory = ReservedMemory = UsedMemory = 0;
    for(unsigned int i = 0; i < memoryMap.EntryCount; i++)
    {
        uint64_t address = entries[i].Address, size = MemoryMapEntry_Size(entries[i]), type = MemoryMapEntry_Type(entries[i]);

        if(address < 0x100000) continue; // Let lower 1 MB of memory be reserved.

        switch(type)
        {
            case MMAP_USED:
            {
                UsedMemory += ((size + 4095) / 4096) * 4096;
                break;
            }

            case MMAP_FREE:
            {
                FreeMemory += ((size + 4095) / 4096) * 4096;
                for(uint64_t addressToFree = address; addressToFree < address + size; addressToFree += 4096)
                {
                    PageFrameBitmap.Clear(addressToFree / 4096);
                }
                break;
            }
        }
    }
    ReservedMemory = memoryMap.MemorySizeKB * 1024 - (UsedMemory + FreeMemory);

    // Mark memory used by bitmap as used.
    uint64_t address = (uint64_t) PageFrameBitmap.Buffer;
    uint64_t size = PageFrameBitmap.BufferSize;
    for(uint64_t addressToFree = address; addressToFree < address + size; addressToFree += 4096)
    {
        PageFrameBitmap.Set(addressToFree / 4096);
    }
    FreeMemory -= ((size + 4095) / 4096) * 4096;
    UsedMemory += ((size + 4095) / 4096) * 4096;
}

void *PageFrameAllocator::RequestPageFrame(void)
{
    while(FirstFreePageFrame < PageFrameBitmap.BufferSize - 1 && PageFrameBitmap.Buffer[FirstFreePageFrame] == 0xFF) 
        FirstFreePageFrame++;

    if(FirstFreePageFrame >= PageFrameBitmap.BufferSize - 1) // Consider at most last 8 page frames of memory as non existent for now
        return nullptr;

    uint64_t index = FirstFreePageFrame << 3; // FirstFreePageFrame * 8
    while(PageFrameBitmap.Get(index) == true) index++;
    PageFrameBitmap.Set(index);
    UsedMemory += 4096;
    FreeMemory -= 4096;

    return (void*) (index * 4096);
}

void PageFrameAllocator::FreePageFrame(void *physicalAddress)
{
    uint64_t index = (uint64_t) physicalAddress / 4096;
    if(PageFrameBitmap.Get(index) == false) 
        return;

    PageFrameBitmap.Clear(index);
    UsedMemory -= 4096;
    FreeMemory += 4096;

    if(FirstFreePageFrame > index / 8) FirstFreePageFrame = index / 8;
}

void *PageFrameAllocator::RequestPageFrames(const uint64_t frames)
{
    while(FirstFreePageFrame < PageFrameBitmap.BufferSize - 1 && PageFrameBitmap.Buffer[FirstFreePageFrame] == 0xFF) 
        FirstFreePageFrame++;

    if(FirstFreePageFrame >= PageFrameBitmap.BufferSize - 1) // Consider at most last 8 page frames of memory as non existent for now
        return nullptr;

    uint64_t index = FirstFreePageFrame << 3; // FirstFreePageFrame * 8
    uint64_t used = 0;
    for(uint64_t i = 0; i < frames; i++)
    {
        if(PageFrameBitmap.Get(index + i)) used += 1;
    }
    while(used > 0)
    {
        if(PageFrameBitmap.Get(index)) used -= 1;
        if(PageFrameBitmap.Get(index + frames)) used += 1;
        index += 1;
    }
    for(uint64_t i = 0; i < frames; i++)
    {
        PageFrameBitmap.Set(index + i);
    }
    UsedMemory += 4096 * frames;
    FreeMemory -= 4096 * frames;

    return (void*) (index * 4096);
}