#include <Display/Renderer.hpp>
#include <Logging.hpp>
#include <Memory/Heap.hpp>
#include <Memory/Memory.hpp>
#include <Memory/PageFrameAllocator.hpp>
#include <Memory/PageTableManager.hpp>
#include <Storage/AHCI.hpp>

namespace AHCI
{
    AHCIDriver *Driver;

    #define HBA_PORT_DEVICE_PRESENT 0x03
    #define HBA_PORT_IPM_ACTIVE 0x01
    #define SATA_SIG_ATAPI 0xEB140101
    #define SATA_SIG_ATA 0x00000101
    #define SATA_SIG_SEMB 0xC33C0101
    #define SATA_SIG_PM 0x96690101

    #define HBA_PORT_CMD_RUNNING 0x8000
    #define HBA_PORT_CMD_FIS_RECEIVE_RUNNING 0x4000
    #define HBA_PORT_CMD_FIS_RECEIVE_ENABLE 0x0010
    #define HBA_PORT_CMD_START 0x0001

    AHCIDriver::AHCIDriver(PCI::DeviceHeader *pciBaseAddress)
    {
        PortCount = 0;
        PCIBaseAddress = pciBaseAddress;
        printf("AHCI Driver initialised.\n");

        PCI::DeviceHeader0 *pci0 = (PCI::DeviceHeader0*) pciBaseAddress;
        pci0->Header.Command &= ~PCI_COMMAND_INT_DISABLE; // Enabling PCI interrupts.

        ABAR = (HBAMemory*) (uint64_t) pci0->BAR[5];

        ABAR->GlobalHostControl |= HBAMEM_GHC_IE; // Enabling HBAMemory interrupts.

        PagingManager.MapPage(ABAR, ABAR, true);

        ProbePorts();

        for(int i = 0; i < PortCount; i++)
        {
            Port *port = Ports[i];
            port->Configure();
            port->Buffer = (uint8_t*) FrameAllocator.RequestPageFrame();
            memset(port->Buffer, 0, 0x1000);
            if(port->Type != PortType::SATA) continue;

            if(!port->Identify(port->Buffer)) // Identify the SATA port.
            {
                errorf("AHCI: Could not identify port %u\n", i);
                continue;
            }

            #ifdef LOGGING
            logf("Port %u IDENTIFY:\n", i);
            uint16_t *identify = (uint16_t*) port->Buffer;
            logf("Hard disk: 0x%x\n", identify[0]);
            logf("LBA48 enable: %u\n", (identify[83] & (1 << 10)) != 0);
            logf("LBA28 sectors: %u\n", *(uint32_t*)(identify + 60));
            uint64_t lba48Sectors = *(uint64_t*)(identify + 100);
            logf("LBA48 sectors: %u\n", lba48Sectors);
            #endif
        }
    }

    AHCIDriver::~AHCIDriver(void)
    {
        for(int i = 0; i < PortCount; i++) delete Ports[i];
    }

    void AHCIDriver::Initialise(PCI::DeviceHeader *pciBaseAddress)
    {
        PortCount = 0;
        PCIBaseAddress = pciBaseAddress;
        printf("AHCI Driver initialised.\n");

        PCI::DeviceHeader0 *pci0 = (PCI::DeviceHeader0*) pciBaseAddress;
        pci0->Header.Command &= ~PCI_COMMAND_INT_DISABLE; // Enabling PCI interrupts.

        ABAR = (HBAMemory*) (uint64_t) pci0->BAR[5];

        ABAR->GlobalHostControl |= HBAMEM_GHC_IE; // Enabling HBAMemory interrupts.

        PagingManager.MapPage(ABAR, ABAR, true);

        ProbePorts();

        for(int i = 0; i < PortCount; i++)
        {
            Port *port = Ports[i];
            port->Configure();
            port->Buffer = (uint8_t*) FrameAllocator.RequestPageFrame();
            memset(port->Buffer, 0, 0x1000);
            if(port->Type != PortType::SATA) continue;

            if(!port->Identify(port->Buffer)) // Identify the SATA port.
            {
                errorf("AHCI: Could not identify port %u\n", i);
                continue;
            }

            #ifdef LOGGING
            logf("Port %u IDENTIFY:\n", i);
            uint16_t *identify = (uint16_t*) port->Buffer;
            logf("Hard disk: 0x%x\n", identify[0]);
            logf("LBA48 enable: %u\n", (identify[83] & (1 << 10)) != 0);
            logf("LBA28 sectors: %u\n", *(uint32_t*)(identify + 60));
            uint64_t lba48Sectors = *(uint64_t*)(identify + 100);
            logf("LBA48 sectors: %u\n", lba48Sectors);
            #endif
        }
    }

    void AHCIDriver::ProbePorts(void)
    {
        uint32_t portsImplemented = ABAR->PortsImplemented;

        for(int i = 0; i < 32; i++)
        {
            if(portsImplemented & (1 << i))
            {
                PortType portType = CheckPortType(&ABAR->Ports[i]);

                switch(portType)
                {
                    case PortType::SATA:
                    case PortType::SATAPI:
                        //Ports[PortCount] = new Port(&ABAR->Ports[i], portType, PortCount);
                        Ports[PortCount] = (AHCI::Port*) KernelHeap.Malloc(sizeof(AHCI::Port));
                        Ports[PortCount]->Init(&ABAR->Ports[i], portType, PortCount);
                        PortCount += 1;
                        break;

                    default: break;
                }
            }
        }
    }

    PortType AHCIDriver::CheckPortType(HBAPort *port)
    {
        uint32_t sataStatus = port->SATAStatus;
        uint8_t interfacePowerManagement = (sataStatus >> 8) & 0b111;
        uint8_t deviceDetection = sataStatus & 0b111;

        if(deviceDetection != HBA_PORT_DEVICE_PRESENT) return PortType::None;
        if(interfacePowerManagement != HBA_PORT_IPM_ACTIVE) return PortType::None;

        switch(port->Signature)
        {
            case SATA_SIG_ATAPI: return PortType::SATAPI;
            case SATA_SIG_ATA: return PortType::SATA;
            case SATA_SIG_SEMB: return PortType::SEMB;
            case SATA_SIG_PM: return PortType::PM;
            default: return PortType::None;
        }
    }


    // Port Class

    Port::Port(HBAPort *port, PortType type, uint8_t portNumber) : hbaPort(port), Type(type), PortNumber(portNumber)
    {

    }

    void Port::Init(HBAPort *port, PortType type, uint8_t portNumber)
    {
        hbaPort = port;
        Type = type;
        PortNumber = portNumber;
    }

    Port::~Port(void)
    {

    }

    void Port::Configure(void)
    {
        StopCommand();

        void *cmdBase = FrameAllocator.RequestPageFrame();
        hbaPort->CommandListBase = (uint32_t) ((uint64_t) cmdBase & 0xFFFFFFFF);
        hbaPort->CommandListBaseUpper = (uint32_t) ((uint64_t) cmdBase >> 32);
        memset(cmdBase, 0, 1024);
        
        void *fisBase = FrameAllocator.RequestPageFrame();
        hbaPort->FISBase = (uint32_t) ((uint64_t) fisBase & 0xFFFFFFFF);
        hbaPort->FISBaseUpper = (uint32_t) ((uint64_t) fisBase >> 32);
        memset(fisBase, 0, 256);

        HBACommandHeader *cmdHeader = (HBACommandHeader*) cmdBase;

        for(int i = 0; i < 32; i++)
        {
            cmdHeader[i].PRDTLength = 8;
            void *cmdTableAddress = FrameAllocator.RequestPageFrame();
            uint64_t address = (uint64_t) cmdTableAddress + (i << 8);
            cmdHeader[i].CTDBase = (uint32_t) address;
            cmdHeader[i].CTDBaseUpper = (uint32_t) (address >> 32);
            memset(cmdTableAddress, 0, 256);
        }

        hbaPort->InterruptEnable = HBAPORT_IE;

        StartCommand();
    }

    void Port::StartCommand(void)
    {
        while(hbaPort->CommandStatus & HBA_PORT_CMD_RUNNING); // Cannot proceed until command stops running.

        hbaPort->CommandStatus |= HBA_PORT_CMD_FIS_RECEIVE_ENABLE;
        hbaPort->CommandStatus |= HBA_PORT_CMD_START;
    }

    void Port::StopCommand(void)
    {
        hbaPort->CommandStatus &= ~HBA_PORT_CMD_START;
        hbaPort->CommandStatus &= ~HBA_PORT_CMD_FIS_RECEIVE_ENABLE;

        while((hbaPort->CommandStatus & HBA_PORT_CMD_FIS_RECEIVE_RUNNING) || (hbaPort->CommandStatus & HBA_PORT_CMD_RUNNING));
    }

    bool Port::Identify(void *buffer)
    {
        uint64_t spin = 0;
        while((hbaPort->TaskFileData & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < TIMEOUT)
        {
            spin += 1;
        }
        if(spin == TIMEOUT)
        {
            errorf("AHCI Identify Timeout.\n");
            return false;
        }

        hbaPort->InterruptStatus = (uint32_t) -1; // Clear pending interrupt bits.

        HBACommandHeader *cmdHeader = (HBACommandHeader*) GetFullAddress(hbaPort->CommandListBase, hbaPort->CommandListBaseUpper);
        cmdHeader->CommandFISLength = sizeof(FIS_REG_H2D) / sizeof(uint32_t);
        cmdHeader->Write = 0; // Read operation.
        cmdHeader->PRDTLength = 1;

        HBACommandTable *cmdTable = (HBACommandTable*) GetFullAddress(cmdHeader->CTDBase, cmdHeader->CTDBaseUpper);
        memset(cmdTable, 0, sizeof(HBACommandTable) + (cmdHeader->PRDTLength - 1) * sizeof(HBAPRDTEntry));

        cmdTable->PRDTEntries[0].DatabaseAddress = (uint32_t) (uint64_t) buffer;
        cmdTable->PRDTEntries[0].DatabaseAddressUpper = (uint32_t) ((uint64_t) buffer >> 32);
        cmdTable->PRDTEntries[0].ByteCount = 511; // sectorCount * 512 - 1;
        cmdTable->PRDTEntries[0].InterruptOnCompletion = 1;

        FIS_REG_H2D *cmdFIS = (FIS_REG_H2D*) &cmdTable->CommandFIS;
        memset(cmdFIS, 0, sizeof(FIS_REG_H2D));
        cmdFIS->FISType = FIS_TYPE_REG_H2D;
        cmdFIS->CommandControl = 1; // Command.
        cmdFIS->Command = ATA_CMD_IDENTIFY;

        hbaPort->CommandIssue = 1;

        while(true)
        {
            if(hbaPort->CommandIssue == 0) break;
            if(hbaPort->InterruptStatus & HBA_PxIS_TFES)
            {
                errorf("AHCI Identify Task File Error.\n");
                return false;
            }
        }

        return true;
    }

    bool Port::Read(uint64_t sector, uint16_t sectorCount, void *buffer)
    {
        uint64_t spin = 0;
        while((hbaPort->TaskFileData & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < TIMEOUT)
        {
            spin += 1;
        }
        if(spin == TIMEOUT)
        {
            errorf("AHCI Disk Read Timeout.\n");
            return false;
        }

        hbaPort->InterruptStatus = (uint32_t) -1; // Clear pending interrupt bits.

        HBACommandHeader *cmdHeader = (HBACommandHeader*) GetFullAddress(hbaPort->CommandListBase, hbaPort->CommandListBaseUpper);
        cmdHeader->CommandFISLength = sizeof(FIS_REG_H2D) / sizeof(uint32_t);
        cmdHeader->Write = 0; // Read operation.
        cmdHeader->PRDTLength = 1;

        HBACommandTable *cmdTable = (HBACommandTable*) GetFullAddress(cmdHeader->CTDBase, cmdHeader->CTDBaseUpper);
        memset(cmdTable, 0, sizeof(HBACommandTable) + (cmdHeader->PRDTLength - 1) * sizeof(HBAPRDTEntry));

        cmdTable->PRDTEntries[0].DatabaseAddress = (uint32_t) (uint64_t) buffer;
        cmdTable->PRDTEntries[0].DatabaseAddressUpper = (uint32_t) ((uint64_t) buffer >> 32);
        cmdTable->PRDTEntries[0].ByteCount = (sectorCount << 9) - 1; // sectorCount * 512 - 1;
        cmdTable->PRDTEntries[0].InterruptOnCompletion = 1;

        FIS_REG_H2D *cmdFIS = (FIS_REG_H2D*) &cmdTable->CommandFIS;
        cmdFIS->FISType = FIS_TYPE_REG_H2D;
        cmdFIS->CommandControl = 1; // Command.
        cmdFIS->Command = ATA_CMD_READ_DMA_EX;

        uint32_t sectorLow = (uint32_t) sector;
        uint32_t sectorHigh = (uint32_t) (sector >> 32);

        cmdFIS->LBA0 = (uint8_t) sectorLow;
        cmdFIS->LBA1 = (uint8_t) (sectorLow >> 8);
        cmdFIS->LBA2 = (uint8_t) (sectorLow >> 16);
        cmdFIS->LBA3 = (uint8_t) sectorHigh;
        cmdFIS->LBA4 = (uint8_t) (sectorHigh >> 8);
        cmdFIS->LBA5 = (uint8_t) (sectorHigh >> 16);

        cmdFIS->Device = 1 << 6; // LBA mode.

        cmdFIS->CountLow = (uint8_t) sectorCount;
        cmdFIS->CountHigh = (uint8_t) (sectorCount >> 8);

        hbaPort->CommandIssue = 1;

        while(true)
        {
            if(hbaPort->CommandIssue == 0) break;
            if(hbaPort->InterruptStatus & HBA_PxIS_TFES)
            {
                errorf("AHCI Read Task File Error.\n");
                return false;
            }
        }

        return true;
    }

    bool Port::Write(uint64_t sector, uint16_t sectorCount, void *buffer)
    {
        uint64_t spin = 0;
        while((hbaPort->TaskFileData & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < TIMEOUT)
        {
            spin += 1;
        }
        if(spin == TIMEOUT)
        {
            errorf("AHCI Disk Write Timeout.\n");
            return false;
        }

        hbaPort->InterruptStatus = (uint32_t) -1; // Clear pending interrupt bits.

        HBACommandHeader *cmdHeader = (HBACommandHeader*) GetFullAddress(hbaPort->CommandListBase, hbaPort->CommandListBaseUpper);
        cmdHeader->CommandFISLength = sizeof(FIS_REG_H2D) / sizeof(uint32_t);
        cmdHeader->Write = 1; // Read operation.
        cmdHeader->PRDTLength = 1;

        HBACommandTable *cmdTable = (HBACommandTable*) GetFullAddress(cmdHeader->CTDBase, cmdHeader->CTDBaseUpper);
        memset(cmdTable, 0, sizeof(HBACommandTable) + (cmdHeader->PRDTLength - 1) * sizeof(HBAPRDTEntry));

        cmdTable->PRDTEntries[0].DatabaseAddress = (uint32_t) (uint64_t) buffer;
        cmdTable->PRDTEntries[0].DatabaseAddressUpper = (uint32_t) ((uint64_t) buffer >> 32);
        cmdTable->PRDTEntries[0].ByteCount = (sectorCount << 9) - 1; // sectorCount * 512 - 1;
        cmdTable->PRDTEntries[0].InterruptOnCompletion = 1;

        FIS_REG_H2D *cmdFIS = (FIS_REG_H2D*) &cmdTable->CommandFIS;
        cmdFIS->FISType = FIS_TYPE_REG_H2D;
        cmdFIS->CommandControl = 1; // Command.
        cmdFIS->Command = ATA_CMD_WRITE_DMA_EX;

        uint32_t sectorLow = (uint32_t) sector;
        uint32_t sectorHigh = (uint32_t) (sector >> 32);

        cmdFIS->LBA0 = (uint8_t) sectorLow;
        cmdFIS->LBA1 = (uint8_t) (sectorLow >> 8);
        cmdFIS->LBA2 = (uint8_t) (sectorLow >> 16);
        cmdFIS->LBA3 = (uint8_t) sectorHigh;
        cmdFIS->LBA4 = (uint8_t) (sectorHigh >> 8);
        cmdFIS->LBA5 = (uint8_t) (sectorHigh >> 16);

        cmdFIS->Device = 1 << 6; // LBA mode.

        cmdFIS->CountLow = (uint8_t) sectorCount;
        cmdFIS->CountHigh = (uint8_t) (sectorCount >> 8);

        hbaPort->CommandIssue = 1;

        while(true)
        {
            if(hbaPort->CommandIssue == 0) break;
            if(hbaPort->InterruptStatus & HBA_PxIS_TFES)
            {
                errorf("AHCI Write Task File Error.\n");
                return false;
            }
        }

        return true;
    }
}