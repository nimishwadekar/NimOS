#include "VFS.hpp"
#include "../Display/Renderer.hpp"
#include "../Memory/Memory.hpp"

// Supported File Systems
//#include "FAT.hpp"
#include "Ext2.hpp"

#define DEVICE_MAX 26

// 'A' - EFI System, Normal partitions from 'C'.
FileSystem *FILE_SYSTEMS[DEVICE_MAX];

void SetupFileSystem(Partition *partition, uint8_t device)
{
    FileSystem *newFS = new FileSystem;
    newFS->Part = partition;

    const char *name = nullptr;
    switch(partition->Type)
    {
        case PartitionType::EFISystem:
            name = "EFI SYS ";
            break;

        case PartitionType::FAT:
        {
            name = "FAT     ";
            /* FAT::FATSystem *fat = new FAT::FATSystem(&partition->Entry);
            newFS->FS = fat;
            newFS->Open = FAT::Open;
            newFS->Close = FAT::Close;
            newFS->Read = FAT::Read;
            newFS->Write = FAT::Write; */
            break;
        }

        case PartitionType::EXT2:
        {
            name = "EXT2    ";
            Ext2::Ext2System *ext2 = new Ext2::Ext2System(&partition->Entry);
            newFS->FS = ext2;
            newFS->Open = Ext2::Open;
            newFS->Close = Ext2::Close;
            newFS->Read = Ext2::Read;
            newFS->Write = Ext2::Write;
            newFS->GetChar = Ext2::GetChar;
            break;
        }

        default: return;
    }
    memcpy(name, newFS->Name, 8);

    FILE_SYSTEMS[device - 'A'] = newFS;
}

void VFSInitialize(const DiskInfo *diskInfo)
{
    memset(FILE_SYSTEMS, 0, DEVICE_MAX * sizeof(FileSystem*));

    uint32_t count = diskInfo->PartitionCount;
    if(count > DEVICE_MAX)
    {
        errorf("Number of partitions greater than VFS DEVICE_MAX; truncating to DEVICE_MAX partitions.\n");
        count = DEVICE_MAX;
    }

    Partition *partitions = diskInfo->Partitions;
    for(uint32_t i = 0, vol = 'C'; i < count; i++, vol++)
    {
        if(partitions[i].Type == PartitionType::None) { vol -= 1; continue; }
        if(partitions[i].Type == PartitionType::EFISystem)
        {
            SetupFileSystem(partitions + i, 'A');
            vol -= 1;
            continue;
        }

        SetupFileSystem(partitions + i, vol);
    }
}

FILE VFSOpenFile(const char *fileName)
{
    if(fileName)
    {
        uint8_t device = 'C';
        if(fileName[1] == ':' && fileName[2] == '/')
        {
            device = fileName[0];
            fileName += 3; // Strip volume from file name.
        }

        uint8_t index = device - 'A';
        if(FILE_SYSTEMS[index]) // Call respective FS' open() function.
        {
            FILE file = FILE_SYSTEMS[index]->Open(FILE_SYSTEMS[index]->FS, fileName);
            file.Device = device;
            return file;
        }
    }

    FILE file;
    file.Flags = FS_INVALID;
    return file;
}

int VFSCloseFile(FILE *file)
{
    if(!file) return -1; // No file.
    if((file->Flags & FS_VALID) == 0) return -1; // File not open.

    uint8_t index = file->Device - 'A';
    return FILE_SYSTEMS[index]->Close(FILE_SYSTEMS[index]->FS, file);
}

uint64_t VFSReadFile(FILE *file, void *buffer, const uint64_t length)
{
    if(!file) return 0; // No file.
    if((file->Flags & FS_VALID) == 0) return 0; // File not open.
    if((file->Flags & FS_FILE) == 0) return 0; // Not a file.
    
    uint8_t index = file->Device - 'A';
    return FILE_SYSTEMS[index]->Read(FILE_SYSTEMS[index]->FS, file, buffer, length);
}

uint64_t VFSWriteFile(FILE *file, const void *buffer, const uint64_t length)
{
    if(!file) return 0; // No file.
    if((file->Flags & FS_VALID) == 0) return 0; // File not open.
    if((file->Flags & FS_FILE) == 0) return 0; // Not a file.
    if((file->Flags & (FS_WRITE | FS_APPEND)) == 0) return 0; // Not a writable file.
    
    uint8_t index = file->Device - 'A';
    return FILE_SYSTEMS[index]->Write(FILE_SYSTEMS[index]->FS, file, buffer, length);
}

char VFSGetChar(FILE *file)
{
    if(!file) return 0; // No file.
    if((file->Flags & FS_VALID) == 0) return 0; // File not open.
    if((file->Flags & FS_FILE) == 0) return 0; // Not a file.
    
    uint8_t index = file->Device - 'A';
    return FILE_SYSTEMS[index]->GetChar(FILE_SYSTEMS[index]->FS, file);
}

void VFSRegisterFileSystem(FileSystem *fileSystem, const uint8_t deviceID)
{
    if(deviceID < DEVICE_MAX && fileSystem)
    {
        FILE_SYSTEMS[deviceID] = fileSystem;
    }
}

void VFSUnregisterFileSystem(FileSystem *fileSystem)
{
    if(!fileSystem) return;
    for(int i = 0; i < DEVICE_MAX; i++)
    {
        if(FILE_SYSTEMS[i] == fileSystem)
        {
            FILE_SYSTEMS[i] = 0;
            return;
        }
    }
}

void VFSUnregisterFileSystemByID(const uint8_t deviceID)
{
    if(deviceID >= DEVICE_MAX) return;
    FILE_SYSTEMS[deviceID] = 0;
}