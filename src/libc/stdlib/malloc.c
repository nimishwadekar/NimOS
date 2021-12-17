#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>

#define BLOCK_SIZE 16
#define HEAP_MAX_MB 80

typedef struct _heaphdr
{
    uint64_t size;
    struct _heaphdr *next;
    struct _heaphdr *prev;
    int free;
} heaphdr_t;

static void _heap_merge_next(heaphdr_t *hdr);
static void _heap_merge_prev(heaphdr_t *hdr);
// Splits a segment into 2 with first segment's size as specified. Returns a pointer to the header for the second segment.
static heaphdr_t *_heap_split(heaphdr_t *hdr, uint64_t firstPartSize);


void _heap_initialize(void *heapAddress, uint64_t pageCount);
// Extends the heap by the given size.
static void _heap_extend(uint64_t size);

static void *_heap_start;
static void *_heap_end;
static heaphdr_t *_heap_last_header;


void *malloc(size_t size)
{
	if((size & (BLOCK_SIZE - 1)) > 0) // size % BLOCK_SIZE
    {
        size -= (size & (BLOCK_SIZE - 1));
        size += BLOCK_SIZE;
    }

    if(size == 0) return NULL;

    heaphdr_t *currentSegment = (heaphdr_t*) _heap_start;
    while(currentSegment != NULL)
    {
        if(currentSegment->free)
        {
            if(currentSegment->size > size)
            {
                heaphdr_t *part2 = _heap_split(currentSegment, size);
                currentSegment->free = 0;
                if(part2 == NULL) continue;
                return (void*) ((uint64_t) currentSegment + sizeof(heaphdr_t));
            }
            else if(currentSegment->size == size)
            {
                currentSegment->free = 0;
                return (void*) ((uint64_t) currentSegment + sizeof(heaphdr_t));
            }
        }
        currentSegment = currentSegment->next;
    }

    // If heap max size reached.
    if((uint64_t) _heap_end - (uint64_t) _heap_start + size > HEAP_MAX_MB * 1024 * 1024)
        return NULL;

    // If not enough memory in heap.
    _heap_extend(size);
    return malloc(size);
}


void free(void *ptr)
{
    if(ptr == NULL) return;

	heaphdr_t *segment = (heaphdr_t*) ((uint64_t) ptr - sizeof(heaphdr_t));
    segment->free = 1;
    _heap_merge_next(segment);
    _heap_merge_prev(segment);
}


void *calloc(size_t nmemb, size_t size)
{
    if(!nmemb || !size) return NULL;

	void *ptr = malloc(nmemb * size);
    return memset(ptr, 0, nmemb * size);
}


void *realloc(void *ptr, size_t size)
{
    if(!size) return NULL;
	if(ptr == NULL) return malloc(size);

    free(ptr);
    void *reloc = malloc(size);
    return memmove(reloc, ptr, size);
}



/******************************************************************************/


void _heap_initialize(void *heapAddress, uint64_t pageCount)
{
    _syscall_3(SYS_MMAP, (int64_t) heapAddress, (int64_t) pageCount, 0);

    uint64_t heapSize = pageCount * 0x1000;
    _heap_start = heapAddress;
    _heap_end = (void*) ((uint64_t) heapAddress + heapSize);
    heaphdr_t *startSegmentHeader = (heaphdr_t*) heapAddress;
    startSegmentHeader->size = heapSize - sizeof(heaphdr_t);
    startSegmentHeader->next = NULL;
    startSegmentHeader->prev = NULL;
    startSegmentHeader->free = 1;
    _heap_last_header = startSegmentHeader;
}


static void _heap_extend(uint64_t size)
{
    if((size & (0x1000 - 1)) > 0) // size % 0x1000
    {
        size -= (size & (0x1000 - 1));
        size += 0x1000;
    }

    uint64_t pageCount = size / 0x1000;
    heaphdr_t *newSegment = (heaphdr_t*) _heap_end;
    _syscall_3(SYS_MMAP, (int64_t) _heap_end, (int64_t) pageCount, 0);
    _heap_end = (void*) ((uint64_t) _heap_end + 0x1000 * pageCount);

    newSegment->free = 1;
    newSegment->next = NULL;
    newSegment->prev = _heap_last_header;
    _heap_last_header->next = newSegment;
    newSegment->size = size - sizeof(heaphdr_t);
    _heap_merge_prev(newSegment);
}

static void _heap_merge_next(heaphdr_t *hdr)
{
    if(hdr->next == NULL || !hdr->next->free) return;

    if(hdr->next == _heap_last_header) _heap_last_header = hdr;
    if(hdr->next->next != NULL) hdr->next->next->prev = hdr;
    hdr->size = hdr->size + sizeof(heaphdr_t) + hdr->next->size;
    hdr->next = hdr->next->next;
}


static void _heap_merge_prev(heaphdr_t *hdr)
{
    if(hdr->prev != NULL && hdr->prev->free) _heap_merge_next(hdr->prev);
}


static heaphdr_t *_heap_split(heaphdr_t *hdr, uint64_t firstPartSize)
{
    int64_t splitSegmentSize = hdr->size - firstPartSize - sizeof(heaphdr_t);
    if(splitSegmentSize < BLOCK_SIZE) return NULL;

    heaphdr_t *secondSegment = (heaphdr_t*) ((uint64_t) hdr + sizeof(heaphdr_t) + firstPartSize);
    if(hdr->next) hdr->next->prev = secondSegment;
    secondSegment->next = hdr->next;
    hdr->next = secondSegment;
    secondSegment->prev = hdr;
    secondSegment->size = splitSegmentSize;
    secondSegment->free = 1;

    hdr->size = firstPartSize;
    if(_heap_last_header == hdr) _heap_last_header = hdr->next;
    return secondSegment;
}