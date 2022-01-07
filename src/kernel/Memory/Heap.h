#pragma once

#include <stdint.h>

namespace Heap
{
    struct Segment
    {
        Segment* Next;
        uint64_t Size;
        bool Free;

        Segment* Split(uint64_t SplitSize);
    };

    void Init();

    void* Allocate(uint64_t Size);

    void Free(void* Address);

    void Reserve(uint64_t Size);
}