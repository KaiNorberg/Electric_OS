#pragma once

#include <stdint.h>

namespace STL
{    
    void SetMemory(const void* Dest, uint8_t Value, const uint64_t Count);

    void CopyMemory(const void* Source, void* Dest, const uint64_t Count);
}
