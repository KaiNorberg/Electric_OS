#pragma once

#include "efiMemory.h"

#include <stdint.h>

namespace Memory
{
    uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);

    void SetMemory(void* Start, uint8_t Value, uint64_t Num);

    void Copy(void* Source, void* Dest, uint64_t Count);
}
