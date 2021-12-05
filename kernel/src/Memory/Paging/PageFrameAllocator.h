#pragma once

#include <stdint.h>
#include "../efiMemory.h"

namespace PageFrameAllocator
{
    void FreePage(void* Address);

    void LockPage(void* Address);

    void FreePages(void* Address, uint64_t PageAmount);

    void LockPages(void* Address, uint64_t PageAmount);

    uint64_t GetTotalMemory();

    uint64_t GetFreeMemory();

    uint64_t GetUsedMemory();

    uint64_t GetReservedMemory();

    void* RequestPage();

    void Init(EFI_MEMORY_DESCRIPTOR* MemoryMap, uint64_t MemoryMapSize, uint64_t MemoryMapDescSize);
}
