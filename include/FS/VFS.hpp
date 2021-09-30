#pragma once

#include <stdint.h>
#include <DyanmicArray.hpp>
#include <FS/File.hpp>
#include <Storage/DiskInfo.hpp>

struct FileSystem
{
    char Name[8]; // Not null-terminated.
    Partition *Part;
    void *FS;

    FILE (*Directory)(const char *directoryName);
    void (*Mount)(void);
    FILE (*Open)(void *fs, const char *fileName);
    int (*Close)(void *fs, FILE *file);
    int64_t (*Read)(void *fs, FILE *file, void *buffer, const int64_t length);
    int64_t (*Write)(void *fs, FILE *file, const void *buffer, const int64_t length);
    int (*Flush)(void *fs, FILE *file);
};

extern DynamicArray<FILE*> OpenedFiles;

void VFSInitialize(const DiskInfo *diskInfo);

// Default volume is assumed to be 'C'.

FILE *VFSOpenFile(const char *fileName, const char *mode);
int VFSCloseFile(FILE *file);
int64_t VFSReadFile(FILE *file, void *buffer, const int64_t length);
int64_t VFSWriteFile(FILE *file, const void *buffer, const int64_t length);
int VFSSeekFile(FILE *file, const int64_t offset, const int whence);
int64_t VFSTellFile(FILE *file);
int VFSFlushFile(FILE *file);

void VFSRegisterFileSystem(FileSystem *fileSystem, const uint8_t deviceID);
void VFSUnregisterFileSystem(FileSystem *fileSystem);
void VFSUnregisterFileSystemByID(const uint8_t deviceID);