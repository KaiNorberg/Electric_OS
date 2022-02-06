#include "Memory.h"

namespace STL
{    
    void SetMemory(const void* Dest, uint8_t Value, const uint64_t Count)
    {
        int d0, d1;
        __asm__ __volatile__("rep\n\t" "stosb" : "=&c" (d0), "=&D" (d1) : "a" (Value), "1" (Dest), "0" (Count) : "memory");
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