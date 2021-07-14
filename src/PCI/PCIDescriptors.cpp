#include <stdint.h>
#include "../String.hpp"

namespace PCI
{
    static char StringBuffer[17];
    const char *GetVendorName(uint16_t vendorID)
    {
        switch (vendorID)
        {
            case 0x8086:
                return "Intel Corp.";
            case 0x1022:
                return "AMD";
            case 0x10DE:
                return "NVIDIA Corporation";
            case 0x1234:
                return "QEMU";
        }
        return (const char*) ultoa(vendorID, 16, StringBuffer);
    }

    const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID)
    {
        switch (vendorID)
        {
            case 0x8086: // Intel
                switch(deviceID)
                {
                    case 0x29C0:
                        return "Express DRAM Controller";
                    case 0x2918:
                        return "LPC Interface Controller";
                    case 0x2922:
                        return "6 port SATA Controller [AHCI mode]";
                    case 0x2930:
                        return "SMBus Controller";
                }
                break;
            
            case 0x1234: // QEMU
                switch(deviceID)
                {
                    case 0x1111:
                        return "Virtual Video Controller";
                }
                break;
        }
        return (const char*) ultoa(deviceID, 16, StringBuffer);
    }

    static const char *DeviceClasses[]
    {
        "Unclassified", // 0
        "Mass Storage Controller", // 1
        "Network Controller", // 2
        "Display Controller", // 3
        "Multimedia Controller", // 4
        "Memory Controller", // 5
        "Bridge Device", // 6
        "Simple Communication Controller", // 7
        "Base System Peripheral", // 8
        "Input Device Controller", // 9
        "Docking Station", // A
        "Processor", // B
        "Serial Bus Controller", // C
        "Wireless Controller", // D
        "Intelligent Controller", // E
        "Satellite Communication Controller", // F
        "Encryption Controller", // 10
        "Signal Processing Controller", // 11
        "Processing Accelerator", // 12
        "Non-Essential Instrumentation" // 13
    };

    const char *GetClassName(uint8_t classCode)
    {
        return DeviceClasses[classCode];
    }

    const char* MassStorageControllerSubclassName(uint8_t subclassCode)
    {
        switch (subclassCode)
        {
            case 0x00:
                return "SCSI Bus Controller";
            case 0x01:
                return "IDE Controller";
            case 0x02:
                return "Floppy Disk Controller";
            case 0x03:
                return "IPI Bus Controller";
            case 0x04:
                return "RAID Controller";
            case 0x05:
                return "ATA Controller";
            case 0x06:
                return "Serial ATA";
            case 0x07:
                return "Serial Attached SCSI";
            case 0x08:
                return "Non-Volatile Memory Controller";
            case 0x80:
                return "Other";
        }
        return ultoa(subclassCode, 16, StringBuffer);
    }

    const char* SerialBusControllerSubclassName(uint8_t subclassCode)
    {
        switch (subclassCode)
        {
            case 0x00:
                return "FireWire (IEEE 1394) Controller";
            case 0x01:
                return "ACCESS Bus";
            case 0x02:
                return "SSA";
            case 0x03:
                return "USB Controller";
            case 0x04:
                return "Fibre Channel";
            case 0x05:
                return "SMBus";
            case 0x06:
                return "Infiniband";
            case 0x07:
                return "IPMI Interface";
            case 0x08:
                return "SERCOS Interface (IEC 61491)";
            case 0x09:
                return "CANbus";
            case 0x80:
                return "SerialBusController - Other";
        }
        return ultoa(subclassCode, 16, StringBuffer);
    }

    const char* BridgeDeviceSubclassName(uint8_t subclassCode)
    {
        switch (subclassCode)
        {
            case 0x00:
                return "Host Bridge";
            case 0x01:
                return "ISA Bridge";
            case 0x02:
                return "EISA Bridge";
            case 0x03:
                return "MCA Bridge";
            case 0x04:
                return "PCI-to-PCI Bridge";
            case 0x05:
                return "PCMCIA Bridge";
            case 0x06:
                return "NuBus Bridge";
            case 0x07:
                return "CardBus Bridge";
            case 0x08:
                return "RACEway Bridge";
            case 0x09:
                return "PCI-to-PCI Bridge";
            case 0x0a:
                return "InfiniBand-to-PCI Host Bridge";
            case 0x80:
                return "Other";
        }
        return ultoa(subclassCode, 16, StringBuffer);
    }

    const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode)
    {
        switch (classCode)
        {
            case 0x01:
                return MassStorageControllerSubclassName(subclassCode);
            case 0x03:
                switch (subclassCode)
                {
                    case 0x00:
                        return "VGA Controller";
                }
                break;
            case 0x06:
                return BridgeDeviceSubclassName(subclassCode);
            case 0x0C:
                return SerialBusControllerSubclassName(subclassCode);
        }
        return ultoa(subclassCode, 16, StringBuffer);
    }

    const char* GetProgramInterfaceName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF)
    {
        switch (classCode)
        {
            case 0x01:
                switch (subclassCode)
                {
                    case 0x06:
                        switch (progIF)
                        {
                            case 0x00:
                                return "Vendor Specific Interface";
                            case 0x01:
                                return "AHCI 1.0";
                            case 0x02:
                                return "Serial Storage Bus";
                        }
                        break;
                }
                break;

            case 0x03:
                switch (subclassCode)
                {
                    case 0x00:
                        switch (progIF)
                        {
                            case 0x00:
                                return "VGA Controller";
                            case 0x01:
                                return "8514-Compatible Controller";
                        }
                        break;
                }
                break;

            case 0x0C:
                switch (subclassCode)
                {
                    case 0x03:
                        switch (progIF)
                        {
                            case 0x00:
                                return "UHCI Controller";
                            case 0x10:
                                return "OHCI Controller";
                            case 0x20:
                                return "EHCI (USB2) Controller";
                            case 0x30:
                                return "XHCI (USB3) Controller";
                            case 0x80:
                                return "Unspecified";
                            case 0xFE:
                                return "USB Device (Not a Host Controller)";
                        }
                        break;
                }
                break;
        }
        return ultoa(progIF, 16, StringBuffer);
    }
}
