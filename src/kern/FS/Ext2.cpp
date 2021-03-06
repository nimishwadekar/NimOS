#include <Display/Renderer.hpp>
#include <FS/Ext2.hpp>
#include <Logging.hpp>
#include <Memory/Heap.hpp>
#include <Memory/Memory.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Storage/DiskInfo.hpp>
#include <String.hpp>

namespace Ext2
{
    #define INODE_TABLE_BUF_OFFSET 0x1000
    #define DIR_TABLE_BUF_OFFSET 0x2000
    #define DATA_BUF_OFFSET 0x3000

    Ext2System::Ext2System(GPTEntry *gpt)
    {
        printf("\n");

        LogicalOffset = gpt->StartingLBA;
        DiskPort = DiskInformation.DiskPort;
        BufferPageCount = 4;
        Buffer = (uint8_t*) FrameAllocator.RequestPageFrames(BufferPageCount);
        for(uint32_t i = 0; i < BufferPageCount; i++) 
            PagingManager.MapPage(Buffer + i * 0x1000, Buffer + i * 0x1000, true);
        memset(Buffer, 0, BufferPageCount * 0x1000);

        if(!DiskPort->Read(LogicalOffset + 2, 2, Buffer)) // Read superblock.
        {
            errorf("Could not load Ext2 Superblock.\n");
            return;
        }
        memcpy(Buffer, &superblock, sizeof(Superblock));

        InodeSizeBytes = (superblock.VersionMajor >= 1) ? superblock.Ext.InodeSize : 128;
        BlockSizeBytes = 1024 << superblock.BlockSize;

        printf("EXT2 File System:\n");
        printf("Signature = 0x%x\n", superblock.Signature);
        printf("Version = %u.%u\n", superblock.VersionMajor, superblock.VersionMinor);
        printf("blocks = %u\n", superblock.BlockCount);
        printf("inodes = %u\n", superblock.InodeCount);
        printf("block size = %u\n", BlockSizeBytes);
        printf("fragment size = %u\n", 1024 << superblock.FragmentSize);
        printf("group size = %u blocks, %u inodes\n", superblock.BlocksPerGroup, superblock.InodesPerGroup);

        BlockGroupCount = (superblock.BlockCount + superblock.BlocksPerGroup - 1) / superblock.BlocksPerGroup;
        if(BlockGroupCount != (superblock.InodeCount + superblock.InodesPerGroup - 1) / superblock.InodesPerGroup)
        {
            errorf("Ext2: Discrepency in block group count!\n");
            return;
        }
        BGTableBlock = superblock.SuperblockBlock + 1;
        uint32_t bgEntriesPerBlock = (BlockSizeBytes) / sizeof(BlockGroupDescriptor);
        uint32_t bgTableBlockCount = (BlockGroupCount + bgEntriesPerBlock - 1) / bgEntriesPerBlock;
        uint32_t bgTablePages = bgTableBlockCount * (BlockSizeBytes) / 0x1000;
        BGTable = (BlockGroupDescriptor*) FrameAllocator.RequestPageFrames(bgTablePages);
        for(uint32_t i = 0; i < bgTablePages; i++) PagingManager.MapPage(BGTable, BGTable, true);
        for(uint32_t i = 0; i < bgTableBlockCount; i++) LoadBlock(BGTableBlock + i, BGTable + i * bgEntriesPerBlock);

        printf("\n");
    }

    bool Ext2System::LoadBlock(const uint64_t block, void *buffer)
    {
        uint64_t sectorsPerBlock = (BlockSizeBytes) / 512;
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
        return ((inode - 1) % superblock.InodesPerGroup) * InodeSizeBytes / (BlockSizeBytes);
    }

    Inode *Ext2System::GetInode(const uint32_t inode)
    {
        uint32_t group = InodeGroup(inode);
        BlockGroupDescriptor *bg = BGTable + group;
        uint32_t block = InodeBlock(inode);
        uint32_t index = InodeIndex(inode);
        uint32_t inodesPerBlock = (BlockSizeBytes) / InodeSizeBytes;
        Inode *table = (Inode*) (Buffer + INODE_TABLE_BUF_OFFSET);

        if(!LoadBlock(bg->InodeTableBlock + block, table)) return nullptr;
        return table + (index % inodesPerBlock);
    }

    Directory *Ext2System::FindDirEntry(const char *dirName, const Inode *parent)
    {
        uint64_t size = parent->SizeLower; // No support yet for 4GB+ files.
        uint64_t blocks = (size + BlockSizeBytes - 1) / BlockSizeBytes;
        uint8_t *dirPtr = Buffer + DIR_TABLE_BUF_OFFSET;
        uint32_t len = strlen(dirName);
        for(int i = 0; blocks > 0; blocks--, i++)
        {
            if(!LoadBlock(parent->DirectBlocks[i], dirPtr)) return nullptr;

            uint32_t offset = 0;
            while(offset < BlockSizeBytes)
            {
                Directory *dir = (Directory*) dirPtr;
                if(dir->Inode == 0) return nullptr;
                if(len == dir->NameLength && memcmp(dirName, dir->Name, len) == 0) return dir;
                dirPtr += dir->EntrySize;
                offset += dir->EntrySize;
            }
        }

        // Indirect pointers not yet implemented.

        return nullptr;
    }

    char NameBuffer[FILENAME_MAX_NULL + 1];
    uint8_t DirIndices[FILENAME_MAX_NULL + 1];

    FILE Open(void *fs, const char *filename)
    {
        memset(NameBuffer, 0, FILENAME_MAX_NULL + 1);
        memset(DirIndices, 0, FILENAME_MAX_NULL + 1);

        FILE invalidFile;
        invalidFile.Flags = FS_INVALID;
        int len = ParseFileName(filename, NameBuffer, DirIndices);
        if(len != FILE_NAME_ERR)
        {
            Ext2System *ext2 = (Ext2System*) fs;
            uint8_t dirEntryType = DIR_ENTRY_DIRECTORY;
            int diri = 0;
            Inode *inode = ext2->GetInode(ROOT_DIR_INODE);
            Directory *dir;
            do
            {
                char *dirName = NameBuffer + DirIndices[diri];
                dir = ext2->FindDirEntry(dirName, inode);
                if(!dir) return invalidFile;
                inode = ext2->GetInode(dir->Inode);
                diri += 1;
            } while(DirIndices[diri] != 0);

            Ext2File *newFile = new Ext2File(inode, ext2->BlockSizeBytes);
            uint32_t id = ext2->OpenFiles.Add(newFile);

            FILE file;
            memset(&file, 0, sizeof(FILE));
            file.CurrentBlock = inode->DirectBlocks[0];
            file.ID = id;
            memset(file.Name, 0, FILENAME_MAX_NULL + 1);
            memcpy(filename, file.Name, len);
            file.Length = inode->SizeLower;
            file.Position = 0;
            file.Flags = FS_VALID;
            if(dir->Type == DIR_ENTRY_FILE) file.Flags |= FS_FILE | FS_WRITE;
            else if(dir->Type == DIR_ENTRY_DIRECTORY) file.Flags |= FS_DIRECTORY;
            
            return file;
        }
        return invalidFile;
    }

    int Close(void *fs, FILE *file)
    {
        uint32_t id = file->ID;
        Ext2System *ext2 = (Ext2System*) fs;
        if(!ext2->OpenFiles.Array[id]) return FILE_EOF;
        ext2->OpenFiles.Delete(id);
        return 0;
    }

    int64_t Read(void *fs, FILE *file, void *buffer, const int64_t length)
    {
        if(length == 0) return 0;
        if(file->Position == file->Length) 
        {
            *(char*) buffer = FILE_EOF;
            return 0;
        }
        int64_t len = (int64_t) length;
        int64_t pos = file->Position;
        if(len > file->Length - file->Position) len = file->Length - file->Position;
        int64_t len2 = len;

        Ext2System *ext2 = (Ext2System*) fs;
        uint32_t blockSize = ext2->BlockSizeBytes;
        int64_t part1 = pos % blockSize;
        uint8_t *dataBuf = ext2->Buffer + DATA_BUF_OFFSET;
        uint8_t *buf = (uint8_t*) buffer;
        if(!ext2->LoadBlock(file->CurrentBlock, dataBuf)) return length - len;

        Ext2File *openFile = ext2->OpenFiles.Array[file->ID];

        if(pos % blockSize != 0)
        {
            if(len <= blockSize - part1)
            {
                memcpy(dataBuf + part1, buf, len);
                file->Position += len;
                return len;
            }
            memcpy(dataBuf + part1, buf, blockSize - part1);

            len -= blockSize - part1;
            pos += blockSize - part1;
            buf += blockSize - part1;
            file->CurrentBlock = ext2->GetNextBlock(openFile);
        }

        int64_t fullBlocks = len / blockSize;
        for(int64_t i = 0; i < fullBlocks; i++)
        {
            if(!ext2->LoadBlock(file->CurrentBlock, dataBuf)) return length - len;
            memcpy(dataBuf, buf, blockSize);
            len -= blockSize;
            pos += blockSize;
            buf += blockSize;
            file->CurrentBlock = ext2->GetNextBlock(openFile);
        }

        int64_t part3 = len % blockSize;
        if(part3 > 0)
        {
            if(!ext2->LoadBlock(file->CurrentBlock, dataBuf)) return length - len;
            memcpy(dataBuf, buf, part3);
            len -= part3;
            pos += part3;
            buf += part3;
        }
        file->Position = pos;
        return len2;
    }

    int64_t Write(void *fs, FILE *file, const void *buffer, const int64_t length)
    {
        return 0;
    }

    int Flush(void *fs, FILE *file)
    {
        return 0;
    }

    uint32_t Ext2System::GetNextBlock(Ext2File *file)
    {
        if(!file->Increment()) return 0;
        if(file->Current[3] == 1)
        {
            if(!LoadBlock(file->inode.IndirectBlock3, file->Buf0)) return -1;
            uint32_t lev2 = ((uint32_t*) file->Buf0)[file->Current[2]];
            if(!LoadBlock(lev2, file->Buf0)) return -1;
            uint32_t lev1 = ((uint32_t*) file->Buf0)[file->Current[1]];
            if(!LoadBlock(lev1, file->Buf0)) return -1;
            return ((uint32_t*) file->Buf0)[file->Current[0]];
        }
        else if(file->Current[2] == 1)
        {
            if(!LoadBlock(file->inode.IndirectBlock2, file->Buf0)) return -1;
            uint32_t lev1 = ((uint32_t*) file->Buf0)[file->Current[1]];
            if(!LoadBlock(lev1, file->Buf0)) return -1;
            return ((uint32_t*) file->Buf0)[file->Current[0]];
        }
        else if(file->Current[1] == 1)
        {
            if(!LoadBlock(file->inode.IndirectBlock1, file->Buf0)) return -1;
            return ((uint32_t*) file->Buf0)[file->Current[0]];
        }
        else return file->inode.DirectBlocks[file->Current[0]];
    }


    // EXT2 File.

    Ext2File::Ext2File(Inode *i, uint16_t blockSizeBytes)
    {
        EntryCount = blockSizeBytes / 4;
        memcpy(i, &inode, sizeof(Inode));
        memset(Current, 0, 4 * sizeof(uint16_t));
        Buf0 = (uint8_t*) FrameAllocator.RequestPageFrame();
        PagingManager.MapPage(Buf0, Buf0, true);
        Buf1 = (uint8_t*) FrameAllocator.RequestPageFrame();
        PagingManager.MapPage(Buf1, Buf1, true);
    }

    bool Ext2File::Increment(void)
    {
        uint64_t cur = *(uint64_t*) Current;
        if(cur == ((1UL << 48) | ((EntryCount - 1UL) << 32) | ((EntryCount - 1UL) << 16) | (EntryCount - 1UL))) return false;
        cur += 1;
        if(cur == 12) cur = 1 << 16;
        *(uint64_t*) Current = cur;
        for(int i = 0; i < 3; i++) 
            if(Current[i] == EntryCount)
            {
                Current[i] = 0;
                Current[i + 1] += 1;
            }
        return true;
    }
}