#pragma once

#include <stdint.h>

#define HEAP_START 0x100000000000
#define HEAP_STARTSIZE 0x10 * 4096

namespace Heap
{
    struct Segment
    {
        Segment* Next;
        uint32_t Size;
        bool Free;

        void* GetStart();

        void* GetEnd();

        void Split(uint32_t NewSize);
    };

    Segment* GetFirstSegment();

    void Init();

    uint32_t GetUsedSize();

    uint32_t GetFreeSize();

    uint32_t GetSegmentAmount();

    void* Allocate(uint32_t Size);

    void Free(void* Address);

    void Reserve(uint32_t Size);
}