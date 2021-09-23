#pragma once

#include <stdint.h>
#include "../PCI/PCI.hpp"

namespace AHCI
{
    #define TIMEOUT 1000000

    #define ATA_DEV_BUSY 0x80
    #define ATA_DEV_DRQ 0x08

    #define ATA_CMD_IDENTIFY 0xEC
    #define ATA_CMD_READ_DMA_EX 0x25
    #define ATA_CMD_WRITE_DMA_EX 0x35

    // Task File Error.
    #define HBA_PxIS_TFES (1 << 30)

    enum class PortType
    {
        None = 0,
        SATA = 1, // hard drives
        SEMB = 2,
        PM = 3,
        SATAPI = 4 // optical drives
    };

    #define HBAPORT_IE 0b11111101'11000000'00000000'11111111

    struct HBAPort // Host Bus Adapter
    {
        uint32_t CommandListBase; // 1K-byte aligned.
        uint32_t CommandListBaseUpper;
        uint32_t FISBase; // Frame Information Structure; 256-byte aligned.
        uint32_t FISBaseUpper;
        uint32_t InterruptStatus;
        uint32_t InterruptEnable;
        uint32_t CommandStatus;
        uint32_t Reserved0;
        uint32_t TaskFileData;
        uint32_t Signature;
        uint32_t SATAStatus;
        uint32_t SATAControl;
        uint32_t SATAError;
        uint32_t SATAActive;
        uint32_t CommandIssue;
        uint32_t SATANotification;
        uint32_t FISSwitchControl;
        uint32_t Reserved1[11];
        uint32_t VendorSpecific[4];
    } __attribute__((packed));

    #define HBAMEM_GHC_IE 0x2

    struct HBAMemory
    {
        uint32_t HostCapability;
        uint32_t GlobalHostControl;
        uint32_t InterruptStatus;
        uint32_t PortsImplemented;
        uint32_t Version;
        uint32_t CCCControl; // Command Completion Coalescing Control.
        uint32_t CCCPorts; // Command Completion Coalescing Ports.
        uint32_t EMLocation; // Enclosure Management Location.
        uint32_t EMControl; // Enclosure Management Control.
        uint32_t HostCapabilityExtended;
        uint32_t BIOSHandoffControlStatus;

        uint8_t Reserved0[0xA0 - 0x2C];
        uint8_t VendorSpecific[0x100 - 0xA0];

        HBAPort Ports[1]; // 1 to 32 ports.
    } __attribute__((packed));

    struct HBACommandHeader
    {
        uint8_t CommandFISLength : 5;
        uint8_t ATAPI : 1;
        uint8_t Write : 1; // 0 - Device to Host, 1 - Host to Device.
        uint8_t Prefetchable : 1;

        uint8_t Reset : 1;
        uint8_t BIST : 1;
        uint8_t ClearBusy : 1;
        uint8_t Reserved0 : 1;
        uint8_t PortMultiplier : 4;

        uint16_t PRDTLength; // Physical Region Descriptor Table Length.
        uint32_t PRDByteCount;
        uint32_t CTDBase; //Command Table Descriptor Base.
        uint32_t CTDBaseUpper;
        
        uint32_t Reserved1[4];
    } __attribute__((packed));

    struct HBAPRDTEntry
    {
        uint32_t DatabaseAddress;
        uint32_t DatabaseAddressUpper;
        uint32_t Reserved0;
        
        uint32_t ByteCount : 22; // 4M max
        uint32_t Reserved1 : 9;
        uint32_t InterruptOnCompletion : 1;
    } __attribute__((packed));

    struct HBACommandTable
    {
        uint8_t CommandFIS[64];
        uint8_t ATAPICommand[16];
        uint8_t Reserved0[48];
        HBAPRDTEntry PRDTEntries[1]; // 0 to 65535 entries.
    } __attribute__((packed));

    enum FIS_TYPE
    {
        FIS_TYPE_REG_H2D = 0x27, // Register FIS - Host to Device
        FIS_TYPE_REG_D2H = 0x34, // Register FIS - Device to Host
        FIS_TYPE_DMA_ACT = 0x39, // DMA Activate FIS - Device to Host
        FIS_TYPE_DMA_SETUP = 0x41, // DMA Setup FIS - Bidirectional
        FIS_TYPE_DATA = 0x46, // Data FIS - Bidirectional
        FIS_TYPE_BIST = 0x58, // BIST Activate FIS - Bidirectional
        FIS_TYPE_PIO_SETUP = 0x5F, // PIO Setup FIS - Device to Host
        FIS_TYPE_DEV_BITS = 0xA1 // Set Device Bits FIS - Device to Host
    };

    struct FIS_REG_H2D
    {
        uint8_t FISType;

        uint8_t portMultiplier : 4;
        uint8_t Reserved0 : 3;
        uint8_t CommandControl : 1; // 1 - Command, 0 - Control.

        uint8_t Command;
        uint8_t FeatureLow;

        uint8_t LBA0;
        uint8_t LBA1;
        uint8_t LBA2;
        uint8_t Device;

        uint8_t LBA3;
        uint8_t LBA4;
        uint8_t LBA5;
        uint8_t FeatureHigh;

        uint8_t CountLow;
        uint8_t CountHigh;
        uint8_t ICC; // Isochronous Command Completion.
        uint8_t Control;

        uint8_t Reserved1[4];
    } __attribute__((packed));

    class Port
    {
        public:
        HBAPort *hbaPort;
        PortType Type;
        uint8_t *Buffer;
        uint8_t PortNumber;

        Port(HBAPort *port, PortType type, uint8_t portNumber);
        ~Port(void);
        void Configure(void);
        void StartCommand(void);
        void StopCommand(void);
        bool Identify(void *buffer);
        bool Read(uint64_t sector, uint16_t sectorCount, void *buffer);
        bool Write(uint64_t sector, uint16_t sectorCount, void *buffer);
    };

    class AHCIDriver
    {
        public:
        PCI::DeviceHeader *PCIBaseAddress;
        HBAMemory *ABAR; //AHCI Base Memory Register.
        Port *Ports[32];
        uint8_t PortCount;

        AHCIDriver(PCI::DeviceHeader *pciBaseAddress);
        ~AHCIDriver(void);
        void ProbePorts(void);

        private:
        PortType CheckPortType(HBAPort *port);
    };

    extern AHCIDriver *Driver;
}