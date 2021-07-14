#include <stdint.h>

namespace PCI
{
    const char *DeviceClasses[]
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
}
