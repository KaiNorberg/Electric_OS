#pragma once

#include "../efiMemory.h"
#include "../Memory.h"

#include <stdint.h>

namespace PageAllocator
{
    void Init(EFI_MEMORY_MAP* MemoryMap);

    void* RequestPage();

    uint64_t GetFreeMemory();

    uint64_t GetLockedMemory();

    uint64_t GetReservedMemory();

    uint64_t GetTotalMemory();

    void* LockPage(void* Address);

    void* ReservePage(void* Address);

    void* FreePage(void* Address);

    void LockPages(void* Address, uint64_t Count);

    void ReservePages(void* Address, uint64_t Count);

    void FreePages(void* Address, uint64_t Count);
}