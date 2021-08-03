#pragma once
#include <stdint.h>
#include "../Storage/DiskInfo.hpp"

#define VFS_INVALID 0
#define VFS_VALID 1
#define VFS_FILE (1 << 1)
#define VFS_DIRECTORY (1 << 2)
#define VFS_WRITE (1 << 3)
#define VFS_APPEND (1 << 4)

struct FILE
{
    char Name[64];
    uint32_t Flags;
    uint8_t Device; // Volume, 'A' to 'Z'.
    uint64_t Length;
    uint64_t CurrentCluster;
    uint64_t Position; // Offset in current cluster.
};

struct FileSystem
{
    char Name[8]; // Not null-terminated.
    Partition *Part;

    FILE (*Directory)(const char *directoryName);
    void (*Mount)(void);
    FILE (*Open)(const char *fileName);
    void (*Close)(FILE *file);
    uint64_t (*Read)(FILE *file, void *buffer, const uint64_t length);
    uint64_t (*Write)(FILE *file, const void *buffer, const uint64_t length);
};

void VFSInitialize(const DiskInfo *diskInfo);

// Default volume is assumed to be 'C'.

FILE VFSOpenFile(const char *fileName);
void VFSCloseFile(FILE *file);
uint64_t VFSReadFile(FILE *file, void *buffer, const uint64_t length);
uint64_t VFSWriteFile(FILE *file, const void *buffer, const uint64_t length);
void VFSRegisterFileSystem(FileSystem *fileSystem, const uint8_t deviceID);
void VFSUnregisterFileSystem(FileSystem *fileSystem);
void VFSUnregisterFileSystemByID(const uint8_t deviceID);