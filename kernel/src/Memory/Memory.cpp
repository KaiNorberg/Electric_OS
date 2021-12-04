#include "Memory.h"
#include "BitMap.h"

namespace Memory
{
    uint64_t FreeMemory;
    uint64_t ReservedMemory;
    uint64_t UsedMemory;

    BitMap PageBitMap;

    bool Initialized = false;

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
        uint64_t BitMapSize = ((MemorySize / 4096) / 8) + 1;

        PageBitMap = BitMap(BitMapSize, LargestFreeMemorySegment);
        for (int i = 0; i < PageBitMap.Size(); i++)
        {
            PageBitMap.Set(i, false);
        }
    }

    uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize){

        static uint64_t memorySizeBytes = 0;
        if (memorySizeBytes > 0) return memorySizeBytes;

        for (int i = 0; i < mMapEntries; i++){
            EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
            memorySizeBytes += desc->NumberOfPages * 4096;
        }

        return memorySizeBytes;

    }
}
