#pragma once

#include <stdint.h>
#include "efiMemory.h"

namespace Memory
{
    uint64_t GetTotalSize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapEntries, uint64_t mMapDescSize);
}
