#pragma once

#include "efiMemory.h"

#include <stdint.h>

namespace Memory
{    
    void Set(const void* Dest, uint8_t Value, const uint64_t Count);

    void Copy(const void* Source, void* Dest, const uint64_t Count);
}
