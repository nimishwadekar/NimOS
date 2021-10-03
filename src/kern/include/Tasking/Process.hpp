#pragma once

#include <stdint.h>
#include <DynamicArray.hpp>
#include <FS/File.hpp>

#define PROCESS_STACK_TOP   0xFFC00000000
#define PROCESS_MAX         20
#define PROCESS_FILE_MAX    32

#define STACK_TOP_ADDR      0x4F0000000
#define STACK_SIZE_KB       100
#define HEAP_BASE_ADDR      0x500000000
#define HEAP_MAX_SIZE_MB    80

// Uses copying for task switching.
#define PROCESS_COPY_ADDR   0x700000000
#define PROCESS_COPY_MAX_MB 2

struct ProcessRegs
{
    uint64_t RBX;
    uint64_t RBP;
    uint64_t RSP;
    uint64_t R12;
    uint64_t R13;
    uint64_t R14;
    uint64_t R15;
    uint64_t RFLAGS;
};

struct Process
{
    int ProcessID;
    int *SpawnExitCodeAddr;
    DynamicArray<FILE*> OpenedFiles;
    void *PC;
    void *StackTop;
    void *StackPhysical;
    void *HeapBase;
    uint64_t StartAddr;
    uint64_t PageCount;
    ProcessRegs Regs;

    // -1 implies not attached to any shared memory segment.
    int16_t SharedMemKey;

    //Assumes every program size is less than 2 MB.
    void *DupAddress;
    void *DupPhysAddress;
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
void BackupTopProcess();
void RestoreTopProcess();

#define PROCESS_EXCEPTION_EXIT_CODE -2147

// Used by exception handlers to exit current process.
void ProcessException();