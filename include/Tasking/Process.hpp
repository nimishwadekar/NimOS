#pragma once

#include <stdint.h>
#include <DynamicArray.hpp>
#include <FS/File.hpp>

#define PROCESS_STACK_TOP 0xFFC00000000

struct Process
{
    int ProcessID;
    //uint32_t OpenFileHandles[8];
    DynamicArray<FILE*> OpenedFiles;
    void* PC;
    void* StackTop;
};

extern Process *ProcessTop;

void PushProcess(void *pc, void *stackTop);
Process PopProcess();