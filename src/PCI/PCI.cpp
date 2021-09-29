#include <Display/Renderer.hpp>
#include <Logging.hpp>
#include <Memory/Heap.hpp>
#include <Memory/PageTableManager.hpp>
#include <PCI/PCI.hpp>
#include <Storage/AHCI.hpp>

namespace PCI
{
    void EnumerateFunction(uint64_t deviceAddress, uint8_t function)
    {
        uint64_t offset = (uint64_t) function << 12;
        uint64_t functionAddress = deviceAddress + offset;
        PagingManager.MapPage((void*) functionAddress, (void*) functionAddress, true);
        /* #ifdef LOGGING
        logf("EnumerateFunction(uint64_t, uint8_t) : 0x%x mapped to phys 0x%x for PCI.\n", functionAddress, functionAddress);
        #endif */

        PCI::DeviceHeader *deviceHeader = (PCI::DeviceHeader*) functionAddress;
        if(deviceHeader->DeviceID == 0 || deviceHeader->DeviceID == 0xFFFF) return; // Not valid.

        MainRenderer.Printf("%s / %s / %s / %s / %s\n", 
            PCI::GetVendorName(deviceHeader->VendorID), 
            PCI::GetDeviceName(deviceHeader->VendorID, deviceHeader->DeviceID), 
            PCI::GetClassName(deviceHeader->Class),
            PCI::GetSubclassName(deviceHeader->Class, deviceHeader->Subclass),
            PCI::GetProgramInterfaceName(deviceHeader->Class, deviceHeader->Subclass, deviceHeader->ProgIF));

        switch(deviceHeader->Class)
        {
            case 0x01: // Mass storage controller
            {
                switch(deviceHeader->Subclass)
                {
                    case 0x06: // SATA
                    {
                        switch(deviceHeader->ProgIF)
                        {
                            case 0x01: // AHCI 1.0 device
                            {
                                //AHCI::Driver = new AHCI::AHCIDriver(deviceHeader);
                                AHCI::Driver = (AHCI::AHCIDriver*) KernelHeap.Malloc(sizeof(AHCI::AHCIDriver));
                                AHCI::Driver->Initialise(deviceHeader);
                                break;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    void EnumerateDevice(uint64_t busAddress, uint8_t device)
    {
        uint64_t offset = (uint64_t) device << 15;
        uint64_t deviceAddress = busAddress + offset;
        PagingManager.MapPage((void*) deviceAddress, (void*) deviceAddress, true);
        /* #ifdef LOGGING
        logf("EnumerateDevice(uint64_t, uint8_t) : 0x%x mapped to phys 0x%x for PCI.\n", deviceAddress, deviceAddress);
        #endif */

        PCI::DeviceHeader *deviceHeader = (PCI::DeviceHeader*) deviceAddress;
        if(deviceHeader->DeviceID == 0 || deviceHeader->DeviceID == 0xFFFF) return; // Not valid.
        for(uint8_t function = 0; function < 8; function++) // 8 functions per device.
        {
            EnumerateFunction(deviceAddress, function);
        }
    }

    void EnumerateBus(uint64_t baseAddress, uint8_t bus)
    {
        uint64_t offset = (uint64_t) bus << 20;
        uint64_t busAddress = baseAddress + offset;
        PagingManager.MapPage((void*) busAddress, (void*) busAddress, true);
        /* #ifdef LOGGING
        logf("EnumerateBus(uint64_t, uint8_t) : 0x%x mapped to phys 0x%x for PCI.\n", busAddress, busAddress);
        #endif */

        PCI::DeviceHeader *deviceHeader = (PCI::DeviceHeader*) busAddress;
        if(deviceHeader->DeviceID == 0 || deviceHeader->DeviceID == 0xFFFF) return; // Not valid.
        for(uint8_t device = 0; device < 32; device++) // 32 devices per bus.
        {
            EnumerateDevice(busAddress, device);
        }
    }

    void EnumeratePCI(const ACPI::MCFGHeader *mcfgHeader)
    {
        uint32_t entryCount = (mcfgHeader->Header.Length - sizeof(ACPI::MCFGHeader)) / sizeof(PCI::DeviceConfig);
        PCI::DeviceConfig *devices = (PCI::DeviceConfig*) ((uint64_t) mcfgHeader + sizeof(ACPI::MCFGHeader));
        for(uint32_t i = 0; i < entryCount; i++)
        {
            for(uint8_t bus = devices[i].StartBus; bus < devices[i].EndBus; bus++)
            {
                EnumerateBus(devices[i].BaseAddress, bus);
            }
        }
    }

    void EndPCI(void)
    {
        delete AHCI::Driver;
    }
}
