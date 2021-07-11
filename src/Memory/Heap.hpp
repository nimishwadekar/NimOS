#pragma once

#include <stdint.h>
#include <stddef.h>

#define BLOCK_SIZE 16

struct HeapSegmentHeader
{
    uint64_t Size;
    HeapSegmentHeader *Next;
    HeapSegmentHeader *Prev;
    bool Free;
    
    void MergeNext(void);
    void MergePrev(void);
    HeapSegmentHeader *Split(uint64_t firstPartSize);
};

class Heap
{
    public:
    void *Start;
    void *End;
    HeapSegmentHeader *LastHeader;

    void Initialize(void *heapAddress, uint64_t pageCount);
    // Extends the heap by the given size.
    void Extend(uint64_t size);
    void *Malloc(uint64_t size);
    void Free(void *address);
};

extern Heap KernelHeap;