#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Usermode/ArgV.hpp>

char *ArgvBuffer;

void InitArgvBuffer()
{
    ArgvBuffer = (char*) ARGV_BUFFER_ADDR;
    PagingManager.MapPage(ArgvBuffer, FrameAllocator.RequestPageFrame());
}