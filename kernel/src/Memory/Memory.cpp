#include "Memory.h"
#include "BitMap.h"

namespace Memory
{
    uint64_t FreeMemory;
    uint64_t ReservedMemory;
    uint64_t UsedMemory;

    BitMap PageBitMap;

    bool Initialized = false;

    void UnreservePage(void* Address)
    {
        uint64_t Index = (uint64_t)Address / 4096;
        if (PageBitMap[Index] == false)
        {
            return;
        }
        else
        {
            PageBitMap.Set(Index, false);
            FreeMemory += 4096;
            ReservedMemory -= 4096;
        }
    }

    void ReservePage(void* Address)
    {
        uint64_t Index = (uint64_t)Address / 4096;
        if (PageBitMap[Index] == true)
        {
            return;
        }
        else
        {
            PageBitMap.Set(Index, true);
            FreeMemory -= 4096;
            ReservedMemory += 4096;
        }
    }

    void UnreservePages(void* Address, uint64_t PageAmount)
    {
        for (int i = 0; i < PageAmount; i++)
        {
            UnreservePage((void*)((uint64_t)Address + (i * 4096)));
        }
    }

    void ReservePages(void* Address, uint64_t PageAmount)
    {
        for (int i = 0; i < PageAmount; i++)
        {
            ReservePage((void*)((uint64_t)Address + (i * 4096)));
        }
    }

    void FreePage(void* Address)
    {
        uint64_t Index = (uint64_t)Address / 4096;
        if (PageBitMap[Index] == false)
        {
            return;
        }
        else
        {
            PageBitMap.Set(Index, false);
            FreeMemory += 4096;
            UsedMemory -= 4096;
        }
    }

    void LockPage(void* Address)
    {
        uint64_t Index = (uint64_t)Address / 4096;
        if (PageBitMap[Index] == true)
        {
            return;
        }
        else
        {
            PageBitMap.Set(Index, true);
            FreeMemory -= 4096;
            UsedMemory += 4096;
        }
    }

    void FreePages(void* Address, uint64_t PageAmount)
    {
        for (int i = 0; i < PageAmount; i++)
        {
            FreePage((void*)((uint64_t)Address + (i * 4096)));
        }
    }

    void LockPages(void* Address, uint64_t PageAmount)
    {
        for (int i = 0; i < PageAmount; i++)
        {
            LockPage((void*)((uint64_t)Address + (i * 4096)));
        }
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

    void* RequestPage()
    {
        for (uint64_t i = 0; i < PageBitMap.Size(); i++)
        {
            if (PageBitMap[i] == true)
            {
                continue;
            }

            LockPage((void*)(i * 4096));
            return (void*)(i * 4096);
        }

        return nullptr;
    }

    void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* MemoryMap, uint64_t MemoryMapSize, uint64_t MemoryMapDescSize)
    {
        if (Initialized)
        {
            return;
        }
        Initialized = true;

        uint64_t MemoryMapEntries = MemoryMapSize / MemoryMapDescSize;

        void* LargestFreeMemorySegment = nullptr;
        uint64_t LargestFreeMemorySegmentSize = 0;
        for (int i = 0; i < MemoryMapEntries; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap + (i * MemoryMapDescSize));
            if (Desc->Type == (uint32_t)EFI_MEMORY_TYPE::EfiConventionalMemory)
            {
                if (Desc->NumberOfPages * 4096 > LargestFreeMemorySegmentSize)
                {
                    LargestFreeMemorySegment = Desc->PhysicalStart;
                    LargestFreeMemorySegmentSize = Desc->NumberOfPages * 4096;
                }
            }
        }

        uint64_t MemorySize = GetMemorySize(MemoryMap, MemoryMapEntries, MemoryMapDescSize);
        FreeMemory = MemorySize;

        PageBitMap = BitMap((MemorySize / 4096) + 1, LargestFreeMemorySegment);
        for (int i = 0; i < PageBitMap.Entries(); i++)
        {
            PageBitMap.Set(i, false);
        }

        LockPages(PageBitMap.GetBuffer(), PageBitMap.Size() / 4096 + 1);

        for (int i = 0; i < MemoryMapEntries; i++)
        {
            EFI_MEMORY_DESCRIPTOR* Desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)MemoryMap + (i * MemoryMapDescSize));
            if (Desc->Type != (uint32_t)EFI_MEMORY_TYPE::EfiConventionalMemory)
            {
                ReservePages(Desc->PhysicalStart, Desc->NumberOfPages);
            }
        }
    }

    uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize){

        static uint64_t memorySizeBytes = 0;
        if (memorySizeBytes > 0) return memorySizeBytes;

        for (int i = 0; i < mMapEntries; i++)
        {
            EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
            memorySizeBytes += desc->NumberOfPages * 4096;
        }

        return memorySizeBytes;
    }
}
