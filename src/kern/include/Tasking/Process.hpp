#pragma once

#include <stdint.h>
#include <DynamicArray.hpp>
#include <FS/File.hpp>

#define PROCESS_STACK_TOP   0xFFC00000000
#define PROCESS_MAX         50
#define PROCESS_FILE_MAX    32

#define STACK_TOP_ADDR      0x4F0000000
#define STACK_SIZE_KB       100
#define HEAP_BASE_ADDR      0x500000000
#define HEAP_MAX_SIZE_MB    80

struct Process
{
    int ProcessID;
    DynamicArray<FILE*> OpenedFiles;
    void *PC;
    void *StackTop;
    void *StackPhysical;
    void *HeapBase;
    uint64_t StartAddr;
    uint64_t PageCount;
};

extern Process *ProcessTop;
extern int ProcessCount;

void InitializeProcessManager();
int PushProcess(void *entry, uint64_t startAddr, uint64_t pageCount);
void PopProcess();
Process *PeekProcess();
int AddFileToCurrentProcess(FILE *file);
// Doesn't actually deallocate the file structure from memory.
void RemoveFileFromCurrentProcess(uint32_t handle);