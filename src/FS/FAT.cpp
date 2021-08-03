#include "FAT.hpp"
#include "../Memory/Memory.hpp"
#include "../String.hpp"
#include "../Storage/DiskInfo.hpp"
#include "../Memory/PageFrameAllocator.hpp"
#include "../Memory/PageTableManager.hpp"
#include "../Display/Renderer.hpp"
#include "../Logging.hpp"

namespace FAT
{
    static void GetMountInfo32(MountInfo *mountInfo, BootRecord32 *boot)
    {
        mountInfo->SectorsPerFAT = boot->SectorsPerFAT;
        mountInfo->RootDirSize = 0;
        mountInfo->FirstDataSector = boot->BPB.ReservedSectorCount + boot->BPB.FATCount * boot->SectorsPerFAT;
        mountInfo->DataSectorCount = boot->BPB.LargeSectorCount - mountInfo->FirstDataSector;
        mountInfo->FirstFATSector = boot->BPB.ReservedSectorCount;
    }

    FATSystem::FATSystem(GPTEntry *gpt)
    {
        uint64_t lba = gpt->StartingLBA;
        DiskPort = DiskInformation.DiskPort;
        Buffer = (uint8_t*) FrameAllocator.RequestPageFrame();
        PagingManager.MapPage(Buffer, Buffer);
        DiskPort->Read(lba, 1, Buffer);
        
        MountInfo mountInfo;
        mountInfo.LogicalOffset = gpt->StartingLBA;

        BIOSParameterBlock *bpb = (BIOSParameterBlock*) Buffer;
        uint32_t sectorsPerCluster = bpb->SectorsPerCluster;

        uint32_t sectorCount = bpb->SectorCount;
        if(sectorCount == 0) sectorCount = bpb->LargeSectorCount;
        mountInfo.SectorCount = sectorCount;

        uint32_t clusterCount = sectorCount / sectorsPerCluster;
        mountInfo.ClusterCount = clusterCount;

        if(clusterCount < 4085) mountInfo.Type = FATType::FAT12;
        else if(clusterCount < 65525) mountInfo.Type = FATType::FAT16;
        else if(clusterCount < 268435445) mountInfo.Type = FATType::FAT32;
        else mountInfo.Type = FATType::exFAT;

        switch(mountInfo.Type)
        {
            case FATType::FAT12:
            case FATType::FAT16:
                break;

            case FATType::FAT32:
            {
                GetMountInfo32(&mountInfo, (BootRecord32*) bpb);
                printf("clusters = 0x%x\nsectors = 0x%x\nsec per fat = %u\nfirst fat = %u\nfirst data = %u\ndata sec count = 0x%x\n", mountInfo.ClusterCount
                    , mountInfo.SectorCount, mountInfo.SectorsPerFAT, mountInfo.FirstFATSector, mountInfo.FirstDataSector, mountInfo.DataSectorCount);
                
                for(uint32_t t = 0; t < mountInfo.DataSectorCount; t++) {
                DiskPort->Read(mountInfo.LogicalOffset + mountInfo.FirstDataSector + t, 1, Buffer);
                uint8_t *fat = (uint8_t*) Buffer;
                for(int i = 0; i < 512; i++)
                {
                    //for(int j = i; j < 8; j++) printf("%x ", fat[j]);
                    if(fat[i] != 0) 
                    logf("%u : %u   %x\n", t, i, fat[i]);
                    //printf("\n");
                } logf("\n\n"); }
                printf("\n\n");

                /* DiskPort->Read(mountInfo.LogicalOffset + mountInfo.FirstDataSector, 4, Buffer);
                for(int i = 0; i < 2048; i++) printf("%u ", Buffer[i]);
                printf("\n"); */

                break;
            }

            case FATType::exFAT:
                break;
        }
    }

    FILE Open(void *fat, const char *filename)
    {
        return {};
    }

    int Close(void *fat, FILE *file)
    {
        return 0;
    }

    uint64_t Read(void *fat, FILE *file, void *buffer, const uint64_t length)
    {
        return 0;
    }

    uint64_t Write(void *fat, FILE *file, const void *buffer, const uint64_t length)
    {
        return 0;
    }
}