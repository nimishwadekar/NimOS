#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Tasking/Process.hpp>

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

int PushProcess(void *pc, void *stackTop)
{
    if(ProcessCount >= PROCESS_MAX) return -1;

    Process p;
    p.PC = pc;
    p.StackTop = stackTop;
    p.ProcessID = ProcessCount + 1;

    *ProcessTop = p;
    ProcessTop += 1;
    ProcessCount += 1;
    return 0;
}


Process PopProcess()
{
    ProcessTop -= 1;
    ProcessCount -= 1;
    return *ProcessTop;
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