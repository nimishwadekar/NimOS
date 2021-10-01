#pragma once

#include <stdint.h>
#include <DynamicArray.hpp>
#include <FS/File.hpp>

#define PROCESS_STACK_TOP 0xFFC00000000

#define PROCESS_MAX 50

#define PROCESS_FILE_MAX 32

struct Process
{
    int ProcessID;
    DynamicArray<FILE*> OpenedFiles;
    void *PC;
    void *StackTop;
    uint64_t StartAddr;
    uint64_t EndAddr;
};

extern Process *ProcessTop;
extern int ProcessCount;

void InitializeProcessManager();
int PushProcess(void *pc, void *stackTop, uint64_t startAddr, uint64_t endAddr);
Process PopProcess();
int AddFileToCurrentProcess(FILE *file);
// Doesn't actually deallocate the file structure from memory.
void RemoveFileFromCurrentProcess(uint32_t handle);