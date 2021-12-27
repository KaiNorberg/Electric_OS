#include "PageAllocator.h"

#include "../../Rendering/Renderer.h"
#include "../../String/cstr.h"

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

namespace PageAllocator
{
    uint8_t* PageStatusMap;
    uint64_t PageAmount;

    void Init(EFI_MEMORY_MAP* MemoryMap, Framebuffer* ScreenBuffer, PSF_FONT* PSFFont)
    {   
        PageAmount = 0;
        for (int i = 0; i < MemoryMap->Size / MemoryMap->DescSize; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap->Base + (i * MemoryMap->DescSize));
            PageAmount += Desc->NumberOfPages;
        }

        void* LargestFreeSegment = nullptr;
        uint64_t LargestFreeSegmentSize = 0;
        for (int i = 0; i < MemoryMap->Size / MemoryMap->DescSize; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap->Base + (i * MemoryMap->DescSize));
            if (Desc->Type == (uint32_t)EFI_MEMORY_TYPE::EfiConventionalMemory && LargestFreeSegmentSize < Desc->NumberOfPages * 4096)
            {
                LargestFreeSegment = Desc->PhysicalStart;
                LargestFreeSegmentSize = Desc->NumberOfPages * 4096;
            }
        }
        PageStatusMap = (uint8_t*)LargestFreeSegment; 
        for (int i = 0; i < PageAmount; i++)
        {
            PageStatusMap[i] = false;
        }

        LockPages(PageStatusMap, PageAmount / 4096 + 1);
        for (int i = 0; i < MemoryMap->Size / MemoryMap->DescSize; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap->Base + (i * MemoryMap->DescSize));
            if (Desc->Type != (uint32_t)EFI_MEMORY_TYPE::EfiConventionalMemory)
            {
                LockPages(Desc->PhysicalStart, Desc->NumberOfPages);
            }
        }
        LockPages(&_KernelStart, ((uint64_t)&_KernelEnd - (uint64_t)&_KernelStart) / 4096 + 1);

        for (int i = 0; i < MemoryMap->Size / MemoryMap->DescSize; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap->Base + (i * MemoryMap->DescSize));
            if (Desc->Type != (uint32_t)EFI_MEMORY_TYPE::EfiConventionalMemory)
            {
                LockPages(Desc->PhysicalStart, Desc->NumberOfPages);
            }
        }
           
        LockPages(ScreenBuffer->Base, ScreenBuffer->Size / 4096 + 1);

        LockPage(PSFFont);
    }

    uint64_t GetFreeMemory()
    {
        uint64_t FreeMemory = 0;
        for (int i = 0; i < PageAmount; i++)
        {
            if (PageStatusMap[i] == 0)
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
            if (PageStatusMap[i] == 1)
            {
                LockedMemory += 4096;
            }
        }
        return LockedMemory;
    }

    uint64_t GetTotalMemory()
    {
        uint64_t TotalMemory = 0;
        for (int i = 0; i < PageAmount; i++)
        {
            TotalMemory += 4096;
        }
        return TotalMemory;
    }

    void* RequestPage()
    {
        for (uint64_t i = 0; i < PageAmount; i++)
        {
            if (PageStatusMap[i] == 0)
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
        PageStatusMap[PageIndex] = true;

        return Address;
    }

    void* FreePage(void* Address)
    {
        uint64_t PageIndex = (uint64_t)Address / 4096;
        if (PageIndex > PageAmount)
        {
            return nullptr;
        }
        PageStatusMap[PageIndex] = false;

        return Address;
    }

    void LockPages(void* Address, uint64_t Count)
    {
        for (int i = 0; i < Count; i++)
        {
            LockPage((void*)((uint64_t)Address + i * 4096));
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