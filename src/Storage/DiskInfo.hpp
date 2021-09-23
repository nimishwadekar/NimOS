#pragma once
#include <stdint.h>
#include "GPT.hpp"
#include "AHCI.hpp"

#define MAX_PARTITIONS 10

enum class PartitionType
{
    None = -1,
    EFISystem = 0,
    FAT,
    EXT2
};

class Partition
{
    public:
    GPTEntry Entry;
    PartitionType Type;
};

class DiskInfo
{
    public:
    AHCI::Port *DiskPort;
    GPTHeader Header;
    Partition *Partitions;
    uint32_t PartitionCount;

    bool Initialize(void);
    PartitionType GetPartitionType(GUID *partitionTypeGUID);
};

extern DiskInfo DiskInformation;