#pragma once

#include "efiMemory.h"

#include <stdint.h>

namespace Memory
{
    void Set(void* Start, uint8_t Value, uint64_t Num);

    void Copy(void* Source, void* Dest, uint64_t Count);
}
