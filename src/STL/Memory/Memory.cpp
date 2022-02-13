#include "Memory.h"

namespace STL
{    
    void SetMemory(const void* Dest, uint8_t Value, const uint64_t Count)
    {
        int d0, d1;
        __asm__ __volatile__("rep\n\t" "stosb" : "=&c" (d0), "=&D" (d1) : "a" (Value), "1" (Dest), "0" (Count) : "memory");
    }

    void CopyMemory(void* Source, void* Dest, uint64_t Count)
    {
        const void* FinalAddress = (void*)((uint64_t)Source + Count);

        while ((uint64_t)Source + 64 < (uint64_t)FinalAddress)
        {
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"((uint64_t)Source +  0), "r"((uint64_t)Dest +  0) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"((uint64_t)Source +  8), "r"((uint64_t)Dest +  8) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"((uint64_t)Source + 16), "r"((uint64_t)Dest + 16) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"((uint64_t)Source + 24), "r"((uint64_t)Dest + 24) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"((uint64_t)Source + 32), "r"((uint64_t)Dest + 32) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"((uint64_t)Source + 40), "r"((uint64_t)Dest + 40) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"((uint64_t)Source + 48), "r"((uint64_t)Dest + 48) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"((uint64_t)Source + 56), "r"((uint64_t)Dest + 56) : "memory");

            Source = (void*)((uint64_t)Source + 64);
            Dest = (void*)((uint64_t)Dest + 64);
        }

        while ((uint64_t)Source < (uint64_t)FinalAddress)
        {
            *((uint8_t*)Dest) = *((uint8_t*)Source);

            Source = (void*)((uint64_t)Source + 1);
            Dest = (void*)((uint64_t)Dest + 1);
        }
    }
}