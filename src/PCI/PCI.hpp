#pragma once
#include "../ACPI/ACPI.hpp"

namespace PCI
{
    #define PCI_COMMAND_INT_DISABLE (1 << 10)

    struct DeviceConfig
    {
        uint64_t BaseAddress;
        uint16_t PCISegGroup;
        uint8_t StartBus;
        uint8_t EndBus;
        uint32_t Reserved;
    } __attribute__((packed));

    struct DeviceHeader
    {
        uint16_t VendorID;
        uint16_t DeviceID;
        uint16_t Command;
        uint16_t Status;
        uint8_t RevisionID;
        uint8_t ProgIF; // Program Interface
        uint8_t Subclass;
        uint8_t Class;
        uint8_t CacheLineSize;
        uint8_t LatencyTimer;
        uint8_t HeaderType;
        uint8_t BIST;
    } __attribute__((packed));

    struct DeviceHeader0
    {
        DeviceHeader Header;
        uint32_t BAR[6]; // 0 to 5.
        uint32_t CardbusCISPtr;
        uint16_t SubsystemVendorID;
        uint16_t SubsystemID;
        uint32_t ExpansionROMBaseAddress;
        uint8_t CapabilitiesPtr;
        uint8_t Reserved0;
        uint16_t Reserved1;
        uint32_t Reserved2;
        uint8_t InterruptLine;
        uint8_t InterruptPin;
        uint8_t MinGrant;
        uint8_t MaxLatency;
    } __attribute__((packed));

    void EnumeratePCI(const ACPI::MCFGHeader *mcfgHeader);
    void EndPCI(void);

    const char *GetVendorName(uint16_t vendorID);
    const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID);
    const char *GetClassName(uint8_t classCode);
    const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode);
    const char* GetProgramInterfaceName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF);
}
