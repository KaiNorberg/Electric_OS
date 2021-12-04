#pragma once

#include <stdint.h>
#include "efiMemory.h"

namespace Memory
{
    void ReadEfiMemoryMap(EFI_MEMORY_DESCRIPTOR* MemoryMap, uint64_t MemoryMapSize, uint64_t MemoryMapDescSize);

    uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
}
