#include "ACPI.h"

#include "kernel/Renderer/Renderer.h"
#include "kernel/PCI/PCI.h"

#include "STL/String/cstr.h"

namespace ACPI
{
    RSDP2* RSDP;
    SDTHeader* XSDT;
    MCFGHeader* MCFG;
    
    void Prepare(RSDP2* In_RSDP)
    {
        RSDP = (RSDP2*)In_RSDP;
        XSDT = (SDTHeader*)(RSDP->XSDTAddress);

        MCFG = (MCFGHeader*)FindTable("MCFG");

        for (int j = 0; j < 4; j++)
        {
            Renderer::Print(MCFG->Header.Signature[j]);
        }

        PCI::Enumerate(MCFG);
    }

    void* FindTable(const char* Signature)
    {
        int Entries = (XSDT->Length - sizeof(SDTHeader)) / 8;

        for (int i = 0; i < Entries; i++)
        {
            SDTHeader* NewHeader = (SDTHeader*)*(uint64_t*)((uint64_t)XSDT + sizeof(SDTHeader) + i * 8);

            for (int j = 0; j < 4; j++)
            {
                if (NewHeader->Signature[j] != Signature[j])
                {
                    break;
                }
                else if (j == 3)
                {
                    return NewHeader;
                }
            }
        }

        return nullptr;
    }
}