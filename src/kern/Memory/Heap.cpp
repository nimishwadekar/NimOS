#include <Memory/Heap.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>

Heap KernelHeap;

// HEAP FUNCTIONS

void Heap::InitializeHeap(void *heapAddress, uint64_t pageCount)
{
    void *position = heapAddress;
    for(uint64_t i = 0; i < pageCount; i++)
    {
        void *positionPhys = FrameAllocator.RequestPageFrame();
        PagingManager.MapPage(position, positionPhys, true);
        position = (void*) ((uint64_t) position + 0x1000);
    }

    uint64_t heapSize = pageCount * 0x1000;
    HeapStart = heapAddress;
    HeapEnd = (void*) ((uint64_t) heapAddress + heapSize);
    HeapSegmentHeader *startSegmentHeader = (HeapSegmentHeader*) heapAddress;
    startSegmentHeader->Size = heapSize - sizeof(HeapSegmentHeader);
    startSegmentHeader->Next = NULL;
    startSegmentHeader->Prev = NULL;
    startSegmentHeader->Free = true;
    LastHeader = startSegmentHeader;
}

void *Heap::Malloc(uint64_t size)
{
    if(size % BLOCK_SIZE > 0)
    {
        size -= (size % BLOCK_SIZE);
        size += BLOCK_SIZE;
    }

    if(size == 0) return NULL;

    HeapSegmentHeader *currentSegment = (HeapSegmentHeader*) HeapStart;
    while(currentSegment != NULL)
    {
        if(currentSegment->Free)
        {
            if(currentSegment->Size > size)
            {
                currentSegment->Split(this, size);
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
    ExtendHeap(size);
    return Malloc(size);
}

void Heap::Free(void *address)
{
    HeapSegmentHeader *segment = (HeapSegmentHeader*) ((uint64_t) address - sizeof(HeapSegmentHeader));
    segment->Free = true;
    segment->MergeNext(this);
    segment->MergePrev(this);
}

void Heap::ExtendHeap(uint64_t size)
{
    if((size & (0xFFF)) > 0) // size % 0x1000
    {
        size -= (size & 0xFFF);
        size += 0x1000;
    }

    uint64_t pageCount = size / 0x1000;
    HeapSegmentHeader *newSegment = (HeapSegmentHeader*) HeapEnd;
    for(uint64_t i = 0; i < pageCount; i++)
    {
        void *heapExtensionPhysical = FrameAllocator.RequestPageFrame();
        PagingManager.MapPage(HeapEnd, heapExtensionPhysical, true);
        HeapEnd = (void*) ((uint64_t) HeapEnd + 0x1000);
    }

    newSegment->Free = true;
    newSegment->Next = NULL;
    newSegment->Prev = LastHeader;
    LastHeader->Next = newSegment;
    newSegment->Size = size - sizeof(HeapSegmentHeader);
    newSegment->MergePrev(this);
}


// HEAP SEGMENT HEADER FUNCTIONS

HeapSegmentHeader *HeapSegmentHeader::Split(Heap *heap, uint64_t firstPartSize)
{
    int64_t splitSegmentSize = Size - firstPartSize - sizeof(HeapSegmentHeader);
    if(splitSegmentSize < BLOCK_SIZE) return NULL;

    HeapSegmentHeader *secondSegment = (HeapSegmentHeader*) ((uint64_t) this + sizeof(HeapSegmentHeader) + firstPartSize);
    if(Next != NULL) Next->Prev = secondSegment;
    secondSegment->Next = Next;
    Next = secondSegment;
    secondSegment->Prev = this;
    secondSegment->Size = splitSegmentSize;
    secondSegment->Free = true;

    Size = firstPartSize;
    if(heap->LastHeader == this) heap->LastHeader = Next;
    return secondSegment;
}

void HeapSegmentHeader::MergeNext(Heap *heap)
{
    if(Next == NULL || !Next->Free) return;

    if(Next == heap->LastHeader) heap->LastHeader = this;
    if(Next->Next != NULL) Next->Next->Prev = this;
    Size = Size + sizeof(HeapSegmentHeader) + Next->Size;
    Next = Next->Next;
}

void HeapSegmentHeader::MergePrev(Heap *heap)
{
    if(Prev != NULL && Prev->Free) Prev->MergeNext(heap);
}