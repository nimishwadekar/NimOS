#include "VFS.hpp"
#include "../Display/Renderer.hpp"
#include "../Memory/Memory.hpp"

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
        case PartitionType::EFISystem: name = "EFI SYS "; break;
        case PartitionType::FAT: name = "FAT     "; break;
        case PartitionType::EXT2: name = "EXT2    "; break;
        default: return;
    }
    memcpy(name, newFS->Name, 8);

    // Assign FS specific functions to function pointers.

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