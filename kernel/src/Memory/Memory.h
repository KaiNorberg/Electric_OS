#pragma once

#include <stdint.h>
#include "efiMemory.h"

namespace Memory
{
    void FreePage(void* Address);

    void LockPage(void* Address);

    void FreePages(void* Address, uint64_t PageAmount);

    void LockPages(void* Address, uint64_t PageAmount);

    uint64_t GetFreeMemory();

    uint64_t GetUsedMemory();

    uint64_t GetReservedMemory();

    void* RequestPage();

    void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* MemoryMap, uint64_t MemoryMapSize, uint64_t MemoryMapDescSize);

    uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
}
