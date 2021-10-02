/* 
#include <Display/Renderer.hpp>
#include <FS/FAT.hpp>
#include <Logging.hpp>
#include <Memory/Memory.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Storage/DiskInfo.hpp>
#include <String.hpp>

namespace FAT
{
    static void GetMountInfo32(MountInfo *mountInfo, BootRecord32 *boot)
    {
        mountInfo->SectorsPerFAT = boot->SectorsPerFAT;
        mountInfo->RootDirSize = 0;
        mountInfo->FirstDataSector = boot->BPB.ReservedSectorCount + boot->BPB.FATCount * boot->SectorsPerFAT;
        mountInfo->DataSectorCount = boot->BPB.LargeSectorCount - mountInfo->FirstDataSector;
        mountInfo->FirstFATSector = boot->BPB.ReservedSectorCount;
        mountInfo->FSInfoSector = boot->FSInfoSector;
    }

    FATSystem::FATSystem(GPTEntry *gpt)
    {
        printf("\n");

        uint64_t lba = gpt->StartingLBA;
        DiskPort = DiskInformation.DiskPort;
        Buffer = (uint8_t*) FrameAllocator.RequestPageFrame();
        PagingManager.MapPage(Buffer, Buffer);
        if(!DiskPort->Read(lba, 1, Buffer))
        {
            errorf("Could not initialize FAT.\n");
            return;
        }
        
        MountInfo mountInfo;
        mountInfo.LogicalOffset = gpt->StartingLBA;

        BIOSParameterBlock *bpb = (BIOSParameterBlock*) Buffer;
        uint32_t sectorsPerCluster = bpb->SectorsPerCluster;
        mountInfo.SectorsPerCluster = sectorsPerCluster;

        uint32_t sectorCount = bpb->SectorCount;
        if(sectorCount == 0) sectorCount = bpb->LargeSectorCount;
        mountInfo.SectorCount = sectorCount;

        uint32_t clusterCount = sectorCount / sectorsPerCluster;

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
                printf("FAT32:\nsec per clus = %u\nsectors = 0x%x\nsec per fat = %u\nfirst fat = %u\nfirst data = %u\ndata sec count = 0x%x\nfsinfo sec = %u\n",
                    mountInfo.SectorsPerCluster, mountInfo.SectorCount, mountInfo.SectorsPerFAT,
                    mountInfo.FirstFATSector, mountInfo.FirstDataSector, mountInfo.DataSectorCount, mountInfo.FSInfoSector);
                break;
            }

            case FATType::exFAT:
                break;
        }
        Info = mountInfo;
        printf("\n");

        Open(this, "LongNamedTextFile");
    }

    // Only use when FATSystem *fat is present.
    #define ClusToSec(cluster) (cluster * fat->Info.SectorsPerCluster)
    #define LogToPhys(sector) (sector + fat->Info.LogicalOffset)

    char NameBuffer[FILENAME_MAX_NULL] = {};
    char LFNBuffer[FILENAME_MAX_NULL] = {};
    uint8_t DirIndices[DIRECTORY_MAX_NEST] = {};

    static bool AppendLFN(LongFileName *lfn)
    {
        if(lfn->Attributes == LFN_DELETED) return true; // Deleted LFN entry.

        uint8_t index = lfn->SequenceNumber & 0x1F; // 1-indexed.
        if(index > 5) return false; // Name over 65 characters.

        char lfnPart[13];
        for(int i = 0; i < 5; i++) lfnPart[i] = (char) lfn->Chars0[i];
        for(int i = 0; i < 6; i++) lfnPart[5 + i] = (char) lfn->Chars1[i];
        for(int i = 0; i < 2; i++) lfnPart[11 + i] = (char) lfn->Chars2[i];
        memcpy(lfnPart, LFNBuffer + 13 * (index - 1), 13);
        return true;
    }

    #define IsClusterEOF(cluster) ((cluster & 0xFFFFFF8) == 0xFFFFFF8)

    // Returns directory table entry.
    Directory *FindDirectory(FATSystem *fat, const uint32_t cluster, const char *dirName)
    {
        uint32_t nextCluster = cluster;
        uint8_t *fatBuf = fat->Buffer, *dataBuf = fat->Buffer + 2048;
        uint32_t clusMin = 0, clusMax = 0; // The currently loaded range of cluster entries of the FAT.

        do
        {
            if(nextCluster < clusMin || nextCluster > clusMax) // Out of loaded range.
            {
                uint32_t sector = nextCluster / 128;
                if(!fat->DiskPort->Read(LogToPhys(fat->Info.FirstFATSector + sector), 1, fatBuf)) // Load correct part of FAT.
                {
                    errorf("FAT Table sector read failed.\n");
                    return nullptr;
                }
                clusMin = sector * 128;
                clusMax = clusMin + 128 - 1;
            }
            
            // -2 because first 2 FAT entries are special entries.
            if(!fat->DiskPort->Read(LogToPhys(fat->Info.FirstDataSector + ClusToSec(nextCluster - 2)), fat->Info.SectorsPerCluster, dataBuf))
            {
                errorf("FAT Data sector read failed.\n");
                return nullptr;
            }

            Directory *dir = (Directory*) dataBuf;
            for(int i = 0; i < 16; i++, dir++)
            {
                if(dir->Attributes == ATTR_LFN)
                {
                    if(!AppendLFN((LongFileName*) dir))
                    {
                        errorf("FAT LFN too long.\n");
                        return nullptr;
                    }
                    continue;
                }
                printf("%s\n", LFNBuffer);
                int cmp = strcmp(dirName, LFNBuffer);
                memset(LFNBuffer, 0, FILENAME_MAX_NULL);

                if(cmp == 0) return dir;
            }

            uint32_t *table = (uint32_t*) fatBuf;
            uint32_t index = nextCluster % 128;
            nextCluster = table[index];
        } while(!IsClusterEOF(nextCluster));

        return nullptr;
    }

    FILE Open(void *fs, const char *filename)
    {
        memset(NameBuffer, 0, FILENAME_MAX_NULL);
        memset(LFNBuffer, 0, FILENAME_MAX_NULL);
        memset(DirIndices, 0, DIRECTORY_MAX_NEST);

        FILE invalidFile;
        invalidFile.Flags = FS_INVALID;

        int len = ParseFileName(filename);
        if(len != FILE_NAME_ERR)
        {
            FATSystem *fat = (FATSystem*) fs;

            int diri = 0;
            uint32_t dirCluster = 2; // First 2 entries not valid.
            Directory *dir;
            do
            {
                char *dirName = NameBuffer + DirIndices[diri];
                dir = FindDirectory(fat, dirCluster, dirName);
                if(!dir)
                {
                    errorf("Could not find directory entry.\n");
                    return invalidFile;
                }

                dirCluster = dir->FirstClusterLow | ((uint32_t) dir->FirstClusterHigh << 16);
                printf("next clus = %u\n", dirCluster);
                diri += 1;
            } while(DirIndices[diri] != 0);

            printf("%u\n", dir->Size);

            fat->DiskPort->Read(0x8080 + 8, 1, fat->Buffer);
            for(int i = 0; i < 512; i++) MainRenderer.PutChar(fat->Buffer[i]);
            return {};
        }
        return invalidFile;
    }

    int Close(void *fs, FILE *file)
    {
        return 0;
    }

    uint64_t Read(void *fs, FILE *file, void *buffer, const uint64_t length)
    {
        return 0;
    }

    uint64_t Write(void *fs, FILE *file, const void *buffer, const uint64_t length)
    {
        return 0;
    }
} */