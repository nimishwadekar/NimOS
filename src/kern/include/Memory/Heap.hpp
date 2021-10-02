#pragma once

#include <stdint.h>
#include <stddef.h>

#define BLOCK_SIZE 16

struct HeapSegmentHeader;

class Heap
{
    public:
    void *HeapStart;
    void *HeapEnd;
    HeapSegmentHeader *LastHeader;

    void InitializeHeap(void *heapAddress, uint64_t pageCount);
    // Extends the heap by the given size.
    void ExtendHeap(uint64_t size);
    void *Malloc(uint64_t size);
    void Free(void *address);
};

struct HeapSegmentHeader
{
    uint64_t Size;
    HeapSegmentHeader *Next;
    HeapSegmentHeader *Prev;
    bool Free;
    
    void MergeNext(Heap *heap);
    void MergePrev(Heap *heap);
    // Splits a segment into 2 with first segment's size as specified. Returns a pointer to the header for the second segment.
    HeapSegmentHeader *Split(Heap *heap, uint64_t firstPartSize);
};

extern Heap KernelHeap;

/* void InitializeHeap(void *heapAddress, uint64_t pageCount);
// Extends the heap by the given size.
void ExtendHeap(uint64_t size);
void *Malloc(uint64_t size);
void Free(void *address); */

inline void *operator new(uint64_t size) { return KernelHeap.Malloc(size); }
inline void *operator new[](uint64_t size) { return KernelHeap.Malloc(size); }

inline void operator delete(void *ptr) { KernelHeap.Free(ptr); }
inline void operator delete(void *ptr, uint64_t size) { KernelHeap.Free(ptr); }
inline void operator delete[](void *ptr) { KernelHeap.Free(ptr); }
inline void operator delete[](void *ptr, uint64_t size) { KernelHeap.Free(ptr); }