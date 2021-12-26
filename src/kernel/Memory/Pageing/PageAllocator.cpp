#include "PageAllocator.h"

#include "../../Rendering/Renderer.h"
#include "../../String/cstr.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
extern uint64_t _PageStatusMap;

namespace PageAllocator
{
    enum PageStatus
    {
        Free = 0b00000000,
        Locked = 0b00000001,
        Reserved = 0b00000010
    };

    uint8_t* PageStatusMap;
    uint64_t PageAmount;

    void Init(EFI_MEMORY_MAP* MemoryMap)
    {   
        uint64_t TotalMemory = 0;
        for (int i = 0; i < MemoryMap->Size / MemoryMap->DescSize; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap->Base + (i * MemoryMap->DescSize));
            TotalMemory += Desc->NumberOfPages * 4096;
        }
        PageAmount = TotalMemory / 4096 + 1;

        PageStatusMap = (uint8_t*)&_PageStatusMap; 
        for (int i = 0; i < PageAmount; i++)
        {
            PageStatusMap[i] = (uint8_t)PageStatus::Free;
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
        uint64_t FreeMemory = 0;
        for (int i = 0; i < PageAmount; i++)
        {
            if (PageStatusMap[i] == (uint8_t)PageStatus::Free)
            {
                FreeMemory += 4096;
            }
        }
        return FreeMemory;
    }

    uint64_t GetLockedMemory()
    {
        uint64_t LockedMemory = 0;
        for (int i = 0; i < PageAmount; i++)
        {
            if (PageStatusMap[i] == (uint8_t)PageStatus::Locked)
            {
                LockedMemory += 4096;
            }
        }
        return LockedMemory;
    }

    uint64_t GetReservedMemory()
    {
        uint64_t ReservedMemory = 0;
        for (int i = 0; i < PageAmount; i++)
        {
            if (PageStatusMap[i] == (uint8_t)PageStatus::Reserved)
            {
                ReservedMemory += 4096;
            }
        }
        return ReservedMemory;
    }

    uint64_t GetTotalMemory()
    {
        return PageAmount * 4096;
    }

    void* RequestPage()
    {
        for (uint64_t i = 0; i < PageAmount; i++)
        {
            if (PageStatusMap[i] == (uint8_t)PageStatus::Free)
            {
                return LockPage((void*)(i * 4096));
            }
        }

        return nullptr;
    }

    void* LockPage(void* Address)
    {
        uint64_t PageIndex = (uint64_t)Address / 4096;
        if (PageIndex > PageAmount)
        {
            return nullptr;
        }
        PageStatusMap[PageIndex] = (uint8_t)PageStatus::Locked;

        return Address;
    }

    void* ReservePage(void* Address)
    {
        uint64_t PageIndex = (uint64_t)Address / 4096;
        if (PageIndex > PageAmount)
        {
            return nullptr;
        }
        PageStatusMap[PageIndex] = (uint8_t)PageStatus::Reserved;

        return Address;
    }

    void* FreePage(void* Address)
    {
        uint64_t PageIndex = (uint64_t)Address / 4096;
        if (PageIndex > PageAmount)
        {
            return nullptr;
        }
        PageStatusMap[PageIndex] = (uint8_t)PageStatus::Free;

        return Address;
    }

    void LockPages(void* Address, uint64_t Count)
    {
        for (int i = 0; i < Count; i++)
        {
            LockPage((void*)((uint64_t)Address + i * 4096));
        }
    }

    void ReservePages(void* Address, uint64_t Count)
    {
        for (int i = 0; i < Count; i++)
        {
            ReservePage((void*)((uint64_t)Address + i * 4096));
        }
    }

    void FreePages(void* Address, uint64_t Count)
    {
        for (int i = 0; i < Count; i++)
        {
            FreePage((void*)((uint64_t)Address + i * 4096));
        }
    }
}