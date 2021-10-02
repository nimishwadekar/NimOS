#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Tasking/Process.hpp>

#include <Display/Renderer.hpp>

Process *ProcessTop;
int ProcessCount;

void InitializeProcessManager()
{
    PagingManager.MapPage((void*) PROCESS_STACK_TOP, FrameAllocator.RequestPageFrame());
    ProcessTop = (Process*) PROCESS_STACK_TOP;
    
    Process kernelProcess;
    kernelProcess.ProcessID = 1;
    *ProcessTop = kernelProcess;
    ProcessTop += 1;
    ProcessCount = 1;
}

int PushProcess(void *entry, uint64_t startAddr, uint64_t pageCount)
{
    if(ProcessCount >= PROCESS_MAX) return -1;

    Process p;
    p.PC = entry;
    p.ProcessID = ProcessCount + 1;
    p.StartAddr = startAddr;
    p.PageCount = pageCount;

    p.StackPhysical = FrameAllocator.RequestPageFrames(STACK_SIZE_KB * 1024 / 0x1000);
    if(!p.StackPhysical) return -1;
    p.StackTop = (void*) (STACK_TOP_ADDR - (ProcessCount - 1) * 0x1000);
    for(uint64_t i = 0; i < STACK_SIZE_KB * 1024; i += 0x1000)
        PagingManager.MapPage((uint8_t*) p.StackTop - STACK_SIZE_KB * 1024 + i, (uint8_t*) p.StackPhysical + i);

    p.HeapBase = (void*) (HEAP_BASE_ADDR + HEAP_MAX_SIZE_MB * 1024 * 1024 * (ProcessCount - 1));

    *ProcessTop = p;
    ProcessTop += 1;
    ProcessCount += 1;
    return 0;
}


void PopProcess()
{
    ProcessTop -= 1;
    ProcessCount -= 1;

    for(uint64_t i = 0; i < STACK_SIZE_KB * 1024; i += 0x1000)
        FrameAllocator.FreePageFrame((uint8_t*) ProcessTop->StackPhysical + i);
}


Process *PeekProcess()
{
    return ProcessTop - 1;
}


int AddFileToCurrentProcess(FILE *file)
{
    Process *p = ProcessTop - 1;
    if(p->OpenedFiles.Count >= PROCESS_FILE_MAX) return -1;
    p->OpenedFiles.Add(file);
    return 0;
}


void RemoveFileFromCurrentProcess(uint32_t handle)
{
    Process *p = ProcessTop - 1;
    for(int i = 0; i < p->OpenedFiles.Capacity; i++)
    {
        if(p->OpenedFiles.Array[i] && p->OpenedFiles.Array[i]->Handle == handle)
        {
            p->OpenedFiles.Array[i] = nullptr;
            return;
        }
    }
}