#include <Display/Renderer.hpp>
#include <Logging.hpp>
#include <Memory/Heap.hpp>
#include <Memory/Memory.hpp>
#include <Storage/DiskInfo.hpp>

DiskInfo DiskInformation;

#define SUPPORTED_PARTITION_TYPES_COUNT 3
GUID SupportedPartitionTypeGUIDs[SUPPORTED_PARTITION_TYPES_COUNT]
{
    { 0xC12A7328, 0xF81F, 0x11D2, { 0xBA, 0x4B, 0x00, 0xA0, 0xC9, 0x3E, 0xC9, 0x3B } }, // EFI System
    { 0xEBD0A0A2, 0xB9E5, 0x4433, { 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7 } }, // FAT (Windows Data)
    { 0x0FC63DAF, 0x8483, 0x4772, { 0x8E, 0x79, 0x3D, 0x69, 0xD8, 0x47, 0x7D, 0xE4 } } // Ext2 (Linux Filesystem)
};

bool DiskInfo::Initialize(void)
{
    DiskPort = nullptr;
    for(int i = 0; i < AHCI::Driver->PortCount; i++)
    {
        if(AHCI::Driver->Ports[i]->Type == AHCI::PortType::SATA)
        {
            DiskPort = AHCI::Driver->Ports[i];
            break;
        }
    }
    if(DiskPort == nullptr)
    {
        errorf("No SATA driver found.\n");
        return false;
    }

    DiskPort->Read(1, 1, DiskPort->Buffer); // Read the GPT Header.
    if(memcmp(DiskPort->Buffer, "EFI PART", 8) != 0) // Check signature
    {
        errorf("GPT Header not found.\n");
        return false;
    }
    memcpy(DiskPort->Buffer, &Header, sizeof(GPTHeader));

    PartitionCount = 0;
    Partitions = new Partition[MAX_PARTITIONS];

    uint32_t entryCount = Header.EntryCount;
    uint64_t lba = Header.EntriesStartLBA;

    uint8_t zero[16]{};
    for(uint32_t i = 0; i < entryCount; i += 32, lba += 8)
    {
        DiskPort->Read(lba, 8, DiskPort->Buffer);
        GPTEntry *entry = (GPTEntry*) DiskPort->Buffer;
        for(int j = 0; j < 32; j++, entry++)
        {
            if(i + j >= entryCount) break;
            if(memcmp(&entry->PartitionTypeGUID, zero, 16) == 0) continue;
            memcpy(entry, &Partitions[PartitionCount].Entry, sizeof(GPTEntry));
            Partitions[PartitionCount].Type = GetPartitionType(&Partitions[PartitionCount].Entry.PartitionTypeGUID);
            PartitionCount += 1;
        }
    }

    #ifdef LOGGING
    logf("\nDisk Partition Count = %u\n", PartitionCount);
    for(uint32_t i = 0; i < PartitionCount; i++)
    {
        logf("Partition %u:\nLBA 0x%x - LBA 0x%x\nAttributes = 0x%x\n", 
            i, Partitions[i].Entry.StartingLBA, Partitions[i].Entry.EndingLBA, Partitions[i].Entry.Attributes);

        switch(Partitions[i].Type)
        {
            case PartitionType::EFISystem: logf("EFI System"); break;
            case PartitionType::FAT: logf("FAT"); break;
            case PartitionType::EXT2: logf("Ext2"); break;
            default: logf("Unsupported"); break;
        }
        logf(" Partition\n");

        logf("Name: ");
        for(int j = 0; Partitions[i].Entry.Name[j] != 0; j++) logf("%c", (char) Partitions[i].Entry.Name[j]);
        logf("\n\n");
    }
    #endif
    
    return true;
}

PartitionType DiskInfo::GetPartitionType(GUID *partitionTypeGUID)
{
    for(int i = 0; i < SUPPORTED_PARTITION_TYPES_COUNT; i++)
    {
        if(memcmp(partitionTypeGUID, SupportedPartitionTypeGUIDs + i, sizeof(GUID)) == 0)
        {
            return (PartitionType) i;
        }
    }
    return PartitionType::None;
}