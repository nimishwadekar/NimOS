#pragma once

#include <stdint.h>
#include <FS/File.hpp>
#include <Storage/AHCI.hpp>
#include <Storage/GPT.hpp>

namespace Ext2
{
    #define ROOT_DIR_INODE 2

    struct SuperblockExt
    {
        uint32_t FirstNonReservedInode;
        uint16_t InodeSize;
        uint16_t SuperblockBlockGroup;
        uint32_t OptionalFeatures;
        uint32_t RequiredFeatures;
        uint32_t ReadOnlyFeatures;

        uint8_t FSID[16];
        char VolName[16]; // Null-terminated.
        char VolMountPath[64]; // Null-terminated.
        
        uint32_t CompressionAlgo;
        uint8_t FilePreallocatedBlockCount;
        uint8_t DirPreallocatedBlockCount;
        uint16_t Reserved0;

        uint8_t JournalID[16]; // Same as FSID.
        uint32_t JournalInode;
        uint32_t JournalDevice;
        uint32_t OrphanInodeListHead;
    } __attribute__((packed));

    struct Superblock
    {
        uint32_t InodeCount;
        uint32_t BlockCount;
        uint32_t SuperuserBlockCount;
        uint32_t UnallocatedBlockCount;

        uint32_t UnallocatedInodeCount;
        uint32_t SuperblockBlock;
        uint32_t BlockSize; // Block size in bytes = (1024 << BlockSize).
        uint32_t FragmentSize; // Fragment size in bytes = (1024 << FragmentSize).

        uint32_t BlocksPerGroup;
        uint32_t FragmentsPerGroup;
        uint32_t InodesPerGroup;
        uint32_t LastMountTime;

        uint32_t LastWriteTime;
        uint16_t MountCount; // Since last consistency check.
        uint16_t MountsAllowed; // Till next consistency check.
        uint16_t Signature; // 0xEF53.
        uint16_t FSState;
        uint16_t ErrorAction;
        uint16_t VersionMinor;

        uint32_t TimeOfCheck; // POSIX Time of last consistency check.
        uint32_t ChecksInterval; // POSIX Time interval between forced consistency checks.
        uint32_t CreatorOSID;
        uint32_t VersionMajor;

        uint16_t ReservedUserID;
        uint16_t ReservedGroupID;

        SuperblockExt Ext;
    } __attribute__((packed));

    struct BlockGroupDescriptor
    {
        uint32_t BlockBitmapBlock;
        uint32_t InodeBitmapBlock;
        uint32_t InodeTableBlock;
        uint16_t UnallocatedBlockCount;
        uint16_t UnallocatedInodeCount;
        uint16_t DirectoryCount;
        uint8_t Reserved0[14];
    } __attribute__((packed));

    struct Inode
    {
        uint16_t TypePermissions;
        uint16_t UserID;
        uint32_t SizeLower; // Bytes.
        uint32_t LastAccessTime;
        uint32_t CreationTime;
        uint32_t LastModificationTime;
        uint32_t DeletionTime;
        uint16_t GroupID;
        uint16_t HardLinkCount;
        uint32_t DiskSectorCount; // Excludes inode structure and directory entries to this inode.
        uint32_t Flags;
        uint8_t OSSpecific1[4];

        uint32_t DirectBlocks[12];
        uint32_t IndirectBlock1;
        uint32_t IndirectBlock2;
        uint32_t IndirectBlock3;

        uint32_t GenerationNumber;
        uint32_t FileACL; // Reserved in ver. < 1.
        uint32_t SizeUpper_DirACL; // Reserved in ver. < 1.
        uint32_t FragmentBlock;
        uint8_t OSSpecific2[12];
    } __attribute__((packed));

    #define DIR_ENTRY_UNKNOWN 0
    #define DIR_ENTRY_FILE 1
    #define DIR_ENTRY_DIRECTORY 2
    #define DIR_ENTRY_CHAR_DEV 3
    #define DIR_ENTRY_BLOCK_DEV 4
    #define DIR_ENTRY_FIFO 5
    #define DIR_ENTRY_SOCKET 6
    #define DIR_ENTRY_SYMLINK 7

    struct Directory
    {
        uint32_t Inode;
        uint16_t EntrySize;
        uint8_t NameLength;
        uint8_t Type;
        char Name[1];
    } __attribute__((packed));

    class Ext2File
    {
        public:
        Inode inode;
        uint16_t EntryCount;
        uint16_t Current[4]; // 4-dimensional pointer for current block.
        uint8_t *Buf0;
        uint32_t LoadedBlock0;
        uint8_t *Buf1;
        uint32_t LoadedBlock1;
        

        Ext2File(Inode *i, uint16_t blockSizeBytes);
        bool Increment(void);
    };

    class Ext2System
    {
        public:
        uint64_t LogicalOffset;
        uint32_t BlockGroupCount;
        uint32_t BGTableBlock;
        uint16_t InodeSizeBytes;
        uint16_t BlockSizeBytes;

        Superblock superblock;
        BlockGroupDescriptor *BGTable;
        AHCI::Port *DiskPort;
        uint8_t *Buffer;
        uint32_t BufferPageCount;

        Ext2File *OpenFiles[8];

        Ext2System(GPTEntry *gpt);
        bool LoadBlock(const uint64_t block, void *buffer);
        uint32_t InodeGroup(const uint32_t inode);
        uint32_t InodeIndex(const uint32_t inode);
        uint32_t InodeBlock(const uint32_t inode);
        Inode *GetInode(const uint32_t inode);
        Directory *FindDirEntry(const char *dirName, const Inode *parent);

        // Returns 0 if no next block. Returns unsigned -1 if error.
        uint32_t GetNextBlock(Ext2File *file);
    };

    FILE Open(void *fs, const char *filename);
    int Close(void *fs, FILE *file);
    int64_t Read(void *fs, FILE *file, void *buffer, const int64_t length);

    // Not implemented.
    
    int64_t Write(void *fs, FILE *file, const void *buffer, const int64_t length);
    int Flush(void *fs, FILE *file);
}