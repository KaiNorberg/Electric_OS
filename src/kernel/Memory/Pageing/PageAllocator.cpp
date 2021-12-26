#include "PageAllocator.h"

#include "../../Rendering/Renderer.h"
#include "../../String/cstr.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
extern uint64_t _PageStatusMap;

namespace PageAllocator
{
    bool* PageStatusMap;
    uint64_t PageAmount;

    uint64_t TotalMemory;
    uint64_t FreeMemory;
    uint64_t ReservedMemory;
    uint64_t UsedMemory;

    void Init(EFI_MEMORY_MAP* MemoryMap)
    {   
        TotalMemory = 0;
        for (int i = 0; i < MemoryMap->Size / MemoryMap->DescSize; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap->Base + (i * MemoryMap->DescSize));
            TotalMemory += Desc->NumberOfPages * 4096;
        }
        FreeMemory = TotalMemory;
        UsedMemory = 0;
        ReservedMemory = 0;

        PageAmount = TotalMemory / 4096 + 1;

        PageStatusMap = (bool*)&_PageStatusMap; 
        for (int i = 0; i < PageAmount; i++)
        {
            PageStatusMap[i] = 0;
        }

        LockPages(PageStatusMap, PageAmount / 4096 + 1);

        for (int i = 0; i < MemoryMap->Size / MemoryMap->DescSize; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap->Base + (i * MemoryMap->DescSize));
            if (Desc->Type != (uint32_t)EFI_MEMORY_TYPE::EfiConventionalMemory)
            {
                ReservePages(Desc->PhysicalStart, Desc->NumberOfPages);
            }
        }

        LockPages(&_KernelStart, ((uint64_t)&_KernelEnd - (uint64_t)&_KernelStart) / 4096 + 1);
    }

    uint64_t GetFreeMemory()
    {
        return FreeMemory;
    }

    uint64_t GetUsedMemory()
    {
        return UsedMemory;
    }

    uint64_t GetReservedMemory()
    {
        return ReservedMemory;
    }

    uint64_t GetTotalMemory()
    {
        return TotalMemory;
    }

    void* RequestPage()
    {
        //BROKEN ON REAL HARDWARE
        /*for (uint64_t i = 0; i < PageAmount; i++)
        {
            if (PageStatusMap[i] == false)
            {
                LockPage((void*)(i * 4096));
                return (void*)(i * 4096);
            }
        }*/

        return nullptr;
    }

    void LockPage(void* Address)
    {
        uint64_t PageIndex = (uint64_t)Address / 4096;
        if (PageStatusMap[PageIndex] == true || PageIndex > PageAmount)
        {
            return;
        }
        PageStatusMap[PageIndex] = true;

        FreeMemory -= 4096;
        UsedMemory += 4096;
    }

    void FreePage(void* Address)
    {
        uint64_t PageIndex = (uint64_t)Address / 4096;
        if (PageStatusMap[PageIndex] == false || PageIndex > PageAmount)
        {
            return;
        }
        PageStatusMap[PageIndex] = false;

        FreeMemory += 4096;
        UsedMemory -= 4096;
    }

    void ReservePage(void* Address)
    {
        uint64_t PageIndex = (uint64_t)Address / 4096;
        if (PageStatusMap[PageIndex] == true || PageIndex > PageAmount)
        {
            return;
        }
        PageStatusMap[PageIndex] = true;

        FreeMemory -= 4096;
        ReservedMemory += 4096;
    }

    void UnReservePage(void* Address)
    {
        uint64_t PageIndex = (uint64_t)Address / 4096;
        if (PageStatusMap[PageIndex] == false || PageIndex > PageAmount)
        {
            return;
        }
        PageStatusMap[PageIndex] = false;

        FreeMemory += 4096;
        ReservedMemory -= 4096;
    }

    void LockPages(void* Address, uint64_t Count)
    {
        for (int i = 0; i < Count; i++)
        {
            LockPage(Address + i * 4096);
        }
    }

    void FreePages(void* Address, uint64_t Count)
    {
        for (int i = 0; i < Count; i++)
        {
            FreePage(Address + i * 4096);
        }
    }

    void ReservePages(void* Address, uint64_t Count)
    {
        for (int i = 0; i < Count; i++)
        {
            ReservePage(Address + i * 4096);
        }
    }

    void UnReservePages(void* Address, uint64_t Count)
    {
        for (int i = 0; i < Count; i++)
        {
            UnReservePage(Address + i * 4096);
        }
    }
}