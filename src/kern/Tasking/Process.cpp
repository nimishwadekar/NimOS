#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Tasking/Process.hpp>
#include <Tasking/SharedMemory.hpp>

#include <Display/Renderer.hpp>

Process *ProcessTop;
int ProcessCount;

void InitializeProcessManager()
{
    PagingManager.MapPage((void*) PROCESS_STACK_TOP, FrameAllocator.RequestPageFrame(), true);
    ProcessTop = (Process*) PROCESS_STACK_TOP;
    
    Process kernelProcess;
    kernelProcess.ProcessID = 1;
    *ProcessTop = kernelProcess;
    ProcessTop += 1;
    ProcessCount = 1;

    ShmManager.Initialise();
}


int PushProcess(void *entry, uint64_t startAddr, uint64_t pageCount)
{
    if(ProcessCount >= PROCESS_MAX) return -1;

    Process p;
    p.PC = entry;
    p.ProcessID = ProcessCount + 1;
    p.StartAddr = startAddr;
    p.PageCount = pageCount;
    p.SharedMemKey = -1;

    p.StackPhysical = FrameAllocator.RequestPageFrames(STACK_SIZE_KB * 1024 / 0x1000);
    if(!p.StackPhysical) return -1;
    p.StackTop = (void*) (STACK_TOP_ADDR - (ProcessCount - 1) * STACK_SIZE_KB * 1024);
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


void BackupTopProcess()
{
    Process *p = ProcessTop - 1;
    uint8_t *copyAddr = (uint8_t*) (PROCESS_COPY_ADDR + (ProcessCount - 2) * PROCESS_COPY_MAX_MB * 1024 * 1024);

    uint8_t *copyPhysAddr = (uint8_t*) FrameAllocator.RequestPageFrames(p->PageCount);
    for(uint64_t i = 0; i < p->PageCount; i++) 
        PagingManager.MapPage(copyAddr + i * 0x1000, copyPhysAddr + i * 0x1000);

    memcpy((void*) p->StartAddr, copyAddr, p->PageCount * 0x1000);
    p->DupAddress = copyAddr;
    p->DupPhysAddress = copyPhysAddr;
}


void RestoreTopProcess()
{
    Process *p = ProcessTop - 1;

    memcpy(p->DupAddress, (void*) p->StartAddr, p->PageCount * 0x1000);

    uint8_t *copyPhysAddr = (uint8_t*) p->DupPhysAddress;
    for(uint64_t i = 0; i < p->PageCount; i++)
        FrameAllocator.FreePageFrame(copyPhysAddr + i * 0x1000);
}


void ProcessException()
{
    // Exit syscall with code -2147.
    asm volatile("movq $0x23, %rax\n \
        movq $-2147, %rdi\n \
        syscall\n");
}