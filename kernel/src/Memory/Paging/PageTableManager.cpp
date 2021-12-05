#include "PageTableManager.h"
#include "PageMapIndexer.h"
#include "PageFrameAllocator.h"
#include "../Memory.h"

#include <stdint.h>

namespace PageTableManager
{
    PageTable* PML4;

    void MapMemory(void* VirtualMemory, void* PhysicalMemory)
    {
        PageMapIndexer Indexer = PageMapIndexer((uint64_t)VirtualMemory);
        PageDirEntry PDE;

        PDE = PML4->Entries[Indexer.PDPindex];
        PageTable* PDP;
        if (!PDE.GetFlag(PageTableFlag::Present))
        {
            PDP = (PageTable*)PageFrameAllocator::RequestPage();
            Memory::SetMemory(PDP, 0, 0x1000);
            PDE.SetAddress((uint64_t)PDP >> 12);
            PDE.SetFlag(PageTableFlag::Present, true);
            PDE.SetFlag(PageTableFlag::ReadWrite, true);
            PML4->Entries[Indexer.PDPindex] = PDE;
        }
        else
        {
            PDP = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }


        PDE = PDP->Entries[Indexer.PDindex];
        PageTable* PD;
        if (!PDE.GetFlag(PageTableFlag::Present))
        {
            PD = (PageTable*)PageFrameAllocator::RequestPage();
            Memory::SetMemory(PD, 0, 0x1000);
            PDE.SetAddress((uint64_t)PD >> 12);
            PDE.SetFlag(PageTableFlag::Present, true);
            PDE.SetFlag(PageTableFlag::ReadWrite, true);
            PDP->Entries[Indexer.PDindex] = PDE;
        }
        else
        {
            PD = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }

        PDE = PD->Entries[Indexer.PTindex];
        PageTable* PT;
        if (!PDE.GetFlag(PageTableFlag::Present))
        {
            PT = (PageTable*)PageFrameAllocator::RequestPage();
            Memory::SetMemory(PT, 0, 0x1000);
            PDE.SetAddress((uint64_t)PT >> 12);
            PDE.SetFlag(PageTableFlag::Present, true);
            PDE.SetFlag(PageTableFlag::ReadWrite, true);
            PD->Entries[Indexer.PTindex] = PDE;
        }
        else
        {
            PT = (PageTable*)((uint64_t)PDE.GetAddress() << 12);
        }

        PDE = PT->Entries[Indexer.Pindex];
        PDE.SetAddress((uint64_t)PhysicalMemory >> 12);
        PDE.SetFlag(PageTableFlag::Present, true);
        PDE.SetFlag(PageTableFlag::ReadWrite, true);
        PT->Entries[Indexer.Pindex] = PDE;
    }

    void Init(Framebuffer* ScreenBuffer)
    {
        PML4 = (PageTable*)PageFrameAllocator::RequestPage();
        Memory::SetMemory(PML4, 0, 0x1000);

        for (uint64_t i = (uint64_t)ScreenBuffer->Base; i < (uint64_t)ScreenBuffer->Base + (uint64_t)ScreenBuffer->Size + 0x1000; i += 0x1000)
        {
            MapMemory((void*)i, (void*)i);
        }

        for (uint64_t i = 0; i < PageFrameAllocator::GetTotalMemory(); i += 0x1000)
        {
            MapMemory((void*)i, (void*)i);
        }

        asm("mov %0, %%cr3" : : "r" (PML4));
    }
}
