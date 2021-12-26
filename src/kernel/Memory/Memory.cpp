#include "Memory.h"

#include "../Rendering/Renderer.h"
#include "../String/cstr.h"
#include "../PIT/PIT.h"

namespace Memory
{
    void SetMemory(void* Start, uint8_t Value, uint64_t Num)
    {
        for (uint64_t i = 0; i < Num; i++)
        {
            *(uint8_t*)((uint64_t)Start + i) = Value;
        }
    }

    void Copy(void* Source, void* Dest, uint64_t Count)
    {
        while (Count-- > 0)
        {
            *(uint8_t*)Dest++ = *(uint8_t*)Source++;
        }
    }
}