#include "Memory.h"

namespace STL
{    
    void SetMemory(const void* Dest, uint8_t Value, const uint64_t Count)
    {
        uint64_t Value64 = ((uint64_t)Value << 0) + ((uint64_t)Value << 8) + ((uint64_t)Value << 16) + ((uint64_t)Value << 24) + 
        ((uint64_t)Value << 32) + ((uint64_t)Value << 40) + ((uint64_t)Value << 48) + ((uint64_t)Value << 56);
        
        for (int i = 0; i < Count / 8; i++)
        {
            ((uint64_t*)Dest)[i] = Value;
        }

        for (int i = Count - Count % 8; i < Count; i++)
        {
            ((uint8_t*)Dest)[i] = Value;
        }
    }

    void CopyMemory(void* Source, void* Dest, const uint64_t Count)
    {
        for(int i = 0; i < Count / 8; i++)
        {
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"(Source), "r"(Dest) : "memory");
            Source += 8;
            Dest += 8;
        }

        for (int i = 0; i < Count % 8; i++)
        {
            ((uint8_t*)Dest)[i] = ((uint8_t*)Source)[i];
        }
    }
}