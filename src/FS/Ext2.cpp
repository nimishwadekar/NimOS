#include "Ext2.hpp"
#include "../Memory/Memory.hpp"
#include "../String.hpp"
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
        printf("Version = %u.%u\n", superblock.VersionMajor, superblock.VersionMinor);
        printf("blocks = %u\n", superblock.BlockCount);
        printf("inodes = %u\n", superblock.InodeCount);
        printf("block size = %u\n", 1024 << superblock.BlockSize);
        printf("fragment size = %u\n", 1024 << superblock.FragmentSize);
        printf("group size = %u blocks, %u inodes\n", superblock.BlocksPerGroup, superblock.InodesPerGroup);
        BlockGroupCount = (superblock.BlockCount + superblock.BlocksPerGroup - 1) / superblock.BlocksPerGroup;
        if(BlockGroupCount != (superblock.InodeCount + superblock.InodesPerGroup - 1) / superblock.InodesPerGroup)
        {
            errorf("Ext2: Discrepency in block group count!\n");
            return;
        }
        BGTableBlock = superblock.SuperblockBlock + 1;
        uint32_t bgEntriesPerBlock = (1024 << superblock.BlockSize) / sizeof(BlockGroupDescriptor);
        uint32_t bgTableBlockCount = (BlockGroupCount + bgEntriesPerBlock - 1) / bgEntriesPerBlock;
        uint32_t bgTablePages = bgTableBlockCount * (1024 << superblock.BlockSize) / 0x1000;
        BGTable = (BlockGroupDescriptor*) FrameAllocator.RequestPageFrames(bgTablePages);
        for(uint32_t i = 0; i < bgTablePages; i++) PagingManager.MapPage(BGTable, BGTable);
        for(uint32_t i = 0; i < bgTableBlockCount; i++) LoadBlock(BGTableBlock + i, BGTable + i * bgEntriesPerBlock);

        

        printf("\n");
        // 12 14
    }

    bool Ext2System::LoadBlock(const uint64_t block, void *buffer)
    {
        uint64_t sectorsPerBlock = (1024 << superblock.BlockSize) / 512;
        return DiskPort->Read(LogicalOffset + sectorsPerBlock * block, sectorsPerBlock, buffer);
    }

    uint32_t Ext2System::InodeGroup(const uint32_t inode)
    {
        return (inode - 1) / superblock.InodesPerGroup;
    }
    
    uint32_t Ext2System::InodeIndex(const uint32_t inode)
    {
        return (inode - 1) % superblock.InodesPerGroup;
    }

    uint32_t Ext2System::InodeBlock(const uint32_t inode)
    {
        uint16_t inodeSize = (superblock.VersionMajor >= 1) ? superblock.Ext.InodeSize : 128;
        return ((inode - 1) % superblock.InodesPerGroup) * inodeSize / superblock.BlockSize;
    }

    char NameBuffer[FILENAME_MAX_NULL + 1];
    uint8_t DirIndices[FILENAME_MAX_NULL + 1];

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