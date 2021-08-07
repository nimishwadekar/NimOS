#include "Ext2.hpp"
#include "../Memory/Memory.hpp"
#include "../Storage/DiskInfo.hpp"
#include "../Memory/PageFrameAllocator.hpp"
#include "../Memory/PageTableManager.hpp"
#include "../Display/Renderer.hpp"

namespace Ext2
{
    Ext2System::Ext2System(GPTEntry *gpt)
    {
        printf("\n");

        LogicalOffset = gpt->StartingLBA;
        DiskPort = DiskInformation.DiskPort;
        BufferPageCount = 4;
        Buffer = (uint8_t*) FrameAllocator.RequestPageFrames(BufferPageCount);
        for(uint32_t i = 0; i < BufferPageCount; i++) 
            PagingManager.MapPage(Buffer + i * 0x1000, Buffer + i * 0x1000);
        memset(Buffer, 0, BufferPageCount * 0x1000);

        if(!DiskPort->Read(LogicalOffset + 2, 2, Buffer)) // Read superblock.
        {
            errorf("Could not load Ext2 Superblock.\n");
            return;
        }
        memcpy(Buffer, &superblock, sizeof(Superblock));

        printf("Signature = 0x%x\n", superblock.Signature);
        printf("inodes = %u\n", superblock.InodeCount);
        printf("block size = %u\n", 1024 << superblock.BlockSize);
        printf("fragment size = %u\n", 1024 << superblock.FragmentSize);
        printf("group size = 0x%x\n", superblock.BlocksPerGroup);
        printf("Version = %u.%u\n", superblock.VersionMajor, superblock.VersionMinor);

        printf("\n");
    }

    bool Ext2System::LoadBlock(const uint64_t block, void *buffer)
    {
        uint64_t sectorsPerBlock = (1024 << superblock.BlockSize) / 512;
        return DiskPort->Read(LogicalOffset + sectorsPerBlock * block, sectorsPerBlock, buffer);
    }

    FILE Open(void *fs, const char *filename)
    {
        return {};
    }

    int Close(void *fs, FILE *file)
    {
        return 0;
    }

    uint64_t Read(void *fs, FILE *file, void *buffer, const uint64_t length)
    {
        return 0;
    }

    uint64_t Write(void *fs, FILE *file, const void *buffer, const uint64_t length)
    {
        return 0;
    }
}