#include "Heap.hpp"
#include "PageFrameAllocator.hpp"
#include "PageTableManager.hpp"
#include "../Display/Renderer.hpp"

Heap KernelHeap;

// HEAP FUNCTIONS

void Heap::Initialize(void *heapAddress, uint64_t pageCount)
{
    void *position = heapAddress;
    for(uint64_t i = 0; i < pageCount; i++)
    {
        PagingManager.MapPage(position, FrameAllocator.GetPage());
        position = (void*) ((uint64_t) position + 0x1000);
    }

    uint64_t heapSize = pageCount * 0x1000;

    Start = heapAddress;
    End = (void*) ((uint64_t) heapAddress + heapSize);
    HeapSegmentHeader *startSegmentHeader = (HeapSegmentHeader*) heapAddress;
    startSegmentHeader->Size = heapSize - sizeof(HeapSegmentHeader);
    startSegmentHeader->Next = NULL;
    startSegmentHeader->Prev = NULL;
    startSegmentHeader->Free = true;
    LastHeader = startSegmentHeader;
}

void *Heap::Malloc(uint64_t size)
{
    if((size & (BLOCK_SIZE - 1)) > 0) // size % BLOCK_SIZE
    {
        size -= (size & (BLOCK_SIZE - 1));
        size += BLOCK_SIZE;
    }

    if(size == 0) return NULL;

    HeapSegmentHeader *currentSegment = (HeapSegmentHeader*) Start;
    while(currentSegment != NULL)
    {
        if(currentSegment->Free)
        {
            if(currentSegment->Size > size)
            {
                currentSegment->Split(size);
                currentSegment->Free = false;
                return (void*) ((uint64_t) currentSegment + sizeof(HeapSegmentHeader));
            }
            else if(currentSegment->Size == size)
            {
                currentSegment->Free = false;
                return (void*) ((uint64_t) currentSegment + sizeof(HeapSegmentHeader));
            }
        }
        currentSegment = currentSegment->Next;
    }

    // If not enough memory in heap.
    Extend(size);
    return Malloc(size);
}

void Heap::Free(void *address)
{

}

void Heap::Extend(uint64_t size)
{

}


// HEAP SEGMENT HEADER FUNCTIONS

HeapSegmentHeader *HeapSegmentHeader::Split(uint64_t firstPartSize)
{
    return NULL;
}

void HeapSegmentHeader::MergeNext(void)
{

}

void HeapSegmentHeader::MergePrev(void)
{
    
}