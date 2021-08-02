#include "DiskInfo.hpp"
#include "../Logging.hpp"
#include "../Display/Renderer.hpp"
#include "../Memory/Memory.hpp"
#include "../Memory/Heap.hpp"

DiskInfo DiskInformation;

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

            // Check Partition Type GUID and set FS type.

            PartitionCount += 1;
        }
    }

    #ifdef LOGGING
    logf("\nDisk Partition Count = %u\n", PartitionCount);
    for(uint32_t i = 0; i < PartitionCount; i++)
    {
        logf("Partition %u:\nLBA 0x%x - LBA 0x%x\nAttributes = 0x%x\n", 
            i, Partitions[i].Entry.StartingLBA, Partitions[i].Entry.EndingLBA, Partitions[i].Entry.Attributes);

        GUID *guid =  &Partitions[i].Entry.PartitionTypeGUID;
        logf("Partition Type GUID: %8x-%4x-%4x-", guid->Data0, guid->Data1, guid->Data2);
        for(int i = 0; i < 8; i++) 
        {
            if(i == 2) logf("-");
            logf("%2x", guid->Data3[i]);
        }
        logf("\n");

        logf("Name: ");
        for(int j = 0; Partitions[i].Entry.Name[j] != 0; j++) logf("%c", (char) Partitions[i].Entry.Name[j]);
        logf("\n\n");
    }
    #endif
    
    return true;
}