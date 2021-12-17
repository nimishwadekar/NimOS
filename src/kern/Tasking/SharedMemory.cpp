#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Tasking/SharedMemory.hpp>

SharedMemoryManager ShmManager;

void SharedMemoryManager::Initialise()
{
    for(int i = 0; i < SHM_MAX_SEGS; i++)
    {
        AttachedProcessCount[i] = 0;
        AttachedPageFrames[i] = 0;
    }
}


void *SharedMemoryManager::AttachProcess(uint8_t key)
{
    void *mem = (void*) (SHM_ADDR_BASE + key * SHM_SEG_SIZE);
    if(AttachedProcessCount[key] == 0)
    {
        void *physMem = FrameAllocator.RequestPageFrame();
        if(!physMem) return 0;
        PagingManager.MapPage(mem, physMem);
        AttachedPageFrames[key] = physMem;
    }
    AttachedProcessCount[key] += 1;
    return mem;
}


void SharedMemoryManager::DetachProcess(uint8_t key)
{
    AttachedProcessCount[key] -= 1;
    if(AttachedProcessCount[key] == 0)
    {
        FrameAllocator.FreePageFrame(AttachedPageFrames[key]);
    }
}


void *SharedMemoryManager::GetShmAddress(uint8_t key)
{
    return (void*) (SHM_ADDR_BASE + key * SHM_SEG_SIZE);
}