#include "PageFrameAllocator.hpp"
#include "../Display/Renderer.hpp"

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

void *PageFrameAllocator::GetPage()
{
    while(FirstFreePageFrame < PageFrameBitmap.BufferSize - 1 && PageFrameBitmap.Buffer[FirstFreePageFrame] == 0xFF) FirstFreePageFrame++;

    if(FirstFreePageFrame >= PageFrameBitmap.BufferSize - 1) // Consider at most last 8 page frames of memory as non existent for now
    {
        // Replace with out of memory exception.
        MainRenderer.PrintErrorf("PAGE FRAME ALLOCATOR :  OUT OF MEMORY\n");
        while(true);
    }

    uint64_t index = FirstFreePageFrame << 3; // FirstFreePageFrame * 8
    while(PageFrameBitmap.Get(index) == true) index++;
    PageFrameBitmap.Set(index);
    UsedMemory += 4096;
    FreeMemory -= 4096;
    return (void*) (index * 4096);
}

void PageFrameAllocator::FreePage(void *physicalAddress)
{
    uint64_t index = (uint64_t) physicalAddress / 4096;
    if(PageFrameBitmap.Get(index) == false)
    {
        // Replace with page frame freed twice exception.
        MainRenderer.PrintErrorf("PAGE FRAME ALLOCATOR :  PAGE FRAME AT PHYSICAL ADDRESS 0x%x FREED TWICE.\n", physicalAddress);
        while(true);
    }

    PageFrameBitmap.Clear(index);
    UsedMemory -= 4096;
    FreeMemory += 4096;

    if(FirstFreePageFrame > index / 8) FirstFreePageFrame = index / 8;
}