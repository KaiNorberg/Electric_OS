#include "AHCI.h"
#include "kernel/PCI/PCI.h"
#include "kernel/Memory/Paging/PageTable.h"
#include "kernel/Renderer/Renderer.h"

namespace AHCI
{
    PCIHeader* ACHIDevice;
    HBAMemory* ABAR;

    void Init()
    {
        DeviceHeader* Device;
        PCI::ResetEnumeration();
        while (PCI::Enumerate(Device))
        { 
            if (Device->Class == 0x01 && Device->Subclass == 0x06 && Device->ProgIF == 0x01)
            {
                ACHIDevice = (PCIHeader*)Device;
                break;
            }
        }

        ABAR = (HBAMemory*)ACHIDevice->BAR5;
        PageTableManager::MapAddress(ABAR, ABAR);   

        /*for (int i = 0; i < 32; i++)
        {
            if ((ABAR->PortsImplemented) & (1 << i))
            {
                switch (ABAR->Ports[i].GetPortType())
                {
                case HBAPortType::SATAPI:
                {
                    Renderer::Print("SATAPI FOUND\n\r");
                }
                break;
                case HBAPortType::SATA:
                {
                    Renderer::Print("SATA FOUND\n\r");
                }
                break;
                case HBAPortType::PM:
                {
                    Renderer::Print("PM FOUND\n\r");
                }
                break;
                case HBAPortType::SEMB:
                {
                    Renderer::Print("SEMB FOUND\n\r");
                }
                break;
                default:
                {
                    Renderer::Print("NONE FOUND\n\r");
                }
                break;
                }
            }
        }*/
    }
}