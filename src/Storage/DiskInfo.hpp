#pragma once
#include <stdint.h>
#include "GPT.hpp"
#include "AHCI.hpp"

#define MAX_PARTITIONS 10

class DiskInfo
{
    public:
    AHCI::Port *DiskPort;
    GPTHeader Header;
    GPTEntry *Partitions;
    uint32_t PartitionCount;

    bool Initialize(void);
};

extern DiskInfo DiskInformation;