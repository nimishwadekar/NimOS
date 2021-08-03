#pragma once
#include <stdint.h>
#include "File.hpp"
#include "../Storage/GPT.hpp"
#include "../Storage/AHCI.hpp"

namespace FAT
{
    #define ATTR_READ_ONLY 0x01
    #define ATTR_HIDDEN 0x02
    #define ATTR_SYSTEM 0x04
    #define ATTR_VOLUME_ID 0x08
    #define ATTR_DIRECTORY 0x10
    #define ATTR_ARCHIVE 0x20
    #define ATTR_LFN 0x0F

    enum class FATType
    {
        FAT12, FAT16, FAT32, exFAT
    };

    struct BIOSParameterBlock
    {
        uint8_t Jump[3];
        uint8_t OEM[8];
        uint16_t BytesPerSector;
        uint8_t SectorsPerCluster;
        uint16_t ReservedSectorCount;
        uint8_t FATCount;
        uint16_t RootEntryCount;
        uint16_t SectorCount; // 0 if large.
        uint8_t MediaDescriptor;
        uint16_t SectorsPerFAT; // FAT12 / FAT16 only.
        uint16_t SectorsPerTrack;
        uint16_t HeadsCount; // On Storage Media.
        uint32_t HiddenSectorCount; // LBA of the beginning of the partition.
        uint32_t LargeSectorCount;
    } __attribute__((packed));

    struct BootRecord16
    {
        BIOSParameterBlock BPB;
        uint8_t DriveNumber;
        uint8_t FlagsWindowsNT;
        uint8_t Signature; // 0x28 or 0x29.
        uint32_t VolumeID;
        uint8_t VolumeLabelString[11];
        uint8_t SystemIdentifierString[8];
    } __attribute__((packed));

    struct BootRecord32
    {
        BIOSParameterBlock BPB;
        uint32_t SectorsPerFAT;
        uint16_t Flags;
        uint16_t Version;
        uint32_t RootDirCluster;
        uint16_t FSInfoSector;
        uint16_t BackupBootSector;
        uint8_t Reserved0[12];
        uint8_t DriveNumber;
        uint8_t FlagsWindowsNT;
        uint8_t Signature; // 0x28 or 0x29.
        uint32_t VolumeID;
        uint8_t VolumeLabelString[11];
        uint8_t SystemIdentifierString[8];
    } __attribute__((packed));

    struct FSInfo
    {
        uint32_t Signature0; // 0x41615252.
        uint8_t Reserves0[480];
        uint32_t Signature1; // 0x61417272.
        uint32_t LastKnownFreeClusterCount; // 0xFFFFFFFF if unknown.
        uint32_t AvailableClusterBegin;
        uint8_t Reserved1[12];
        uint32_t Signature2; // 0xAA550000.
    } __attribute__((packed));

    struct FileTime // Multiply seconds by 2.
    {
        uint16_t Hour : 5;
        uint16_t Minutes : 6;
        uint16_t Seconds : 5;
    } __attribute__((packed));

    struct FileDate
    {
        uint16_t Year : 7;
        uint16_t Month : 4;
        uint16_t Day : 5;
    }  __attribute__((packed));

    struct Directory
    {
        uint8_t Name[11];
        uint8_t Attributes;
        uint8_t ReservedNT;
        uint8_t CreationTimeDeciseconds; // [0 - 199].
        FileTime CreationTime;
        FileDate CreationDate;
        FileDate LastAccessedDate;
        uint16_t FirstClusterHigh; // 0 for FAT12 and FAT16.
        FileTime LastModifiedTime;
        FileDate LastModifiedDate;
        uint16_t FirstClusterLow;
        uint32_t Size;
    } __attribute__((packed));

    struct LongFileName
    {
        uint8_t Order; // 1-indexed.
        uint16_t Chars0[5];
        uint8_t Attributes; // 0x0F.
        uint8_t LongEntryType; // 0 for name entries.
        uint8_t Checksum;
        uint16_t Chars1[6];
        uint16_t Zero;
        uint16_t Chars2[2];
    } __attribute__((packed));

    struct MountInfo
    {
        FATType Type;
        uint64_t LogicalOffset;
        uint32_t SectorCount;
        uint32_t ClusterCount;
        uint32_t SectorsPerFAT;
        uint32_t RootDirSize; // In bytes.
        uint32_t FirstDataSector;
        uint32_t DataSectorCount;
        uint32_t FirstFATSector;
    };

    class FATSystem
    {
        public:
        uint8_t BPBMemory[100]; // Cast to respective BPB.
        MountInfo Info;
        AHCI::Port *DiskPort;
        uint8_t *Buffer;

        FATSystem(GPTEntry *gpt);
    };

    FILE Open(void *fat, const char *filename);
    int Close(void *fat, FILE *file);
    uint64_t Read(void *fat, FILE *file, void *buffer, const uint64_t length);
    uint64_t Write(void *fat, FILE *file, const void *buffer, const uint64_t length);
}