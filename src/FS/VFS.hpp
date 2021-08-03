#pragma once
#include <stdint.h>
#include "../Storage/DiskInfo.hpp"

enum class FileType
{
    Invalid = -1,
    File,
    Directory
};

struct FILE
{
    char Name[64];
    FileType Type;
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
    void (*Read)(FILE* file, void *buffer, uint64_t length);
    FILE (*Open)(const char *fileName);
    void (*Close)(FILE *file);
};

void VFSInitialize(const DiskInfo *diskInfo);

FILE VFSOpenFile(const char *fileName);
void VFSReadFile(FILE *file, void *buffer, const uint64_t length);
void VFSCloseFile(FILE *file);
void VFSRegisterFileSystem(FileSystem *fileSystem, const uint8_t deviceID);
void VFSUnregisterFileSystem(FileSystem *fileSystem);
void VFSUnregisterFileSystemByID(const uint8_t deviceID);