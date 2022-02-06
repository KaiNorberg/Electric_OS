#include "ACPI.h"

#include "kernel/Renderer/Renderer.h"
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

        //Renderer::Print(STL::ToString((uint64_t)MCFG));

        //Renderer::SwapBuffers();
    }

    void* FindTable(const char* Signature)
    {
        int Entries = (XSDT->Length - sizeof(SDTHeader)) / 8;

        for (int i = 0; i < Entries; i++)
        {
            SDTHeader* NewHeader = (SDTHeader*)*(uint64_t*)((uint64_t)XSDT + sizeof(SDTHeader) + i * 8);

            for (int j = 0; j < 4; j++)
            {
                Renderer::Print(NewHeader->Signature[j]);

            }
            Renderer::Print(" ");
        }

        return nullptr;
    }
}