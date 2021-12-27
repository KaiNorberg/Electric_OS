#pragma once

#include "../efiMemory.h"
#include "../Memory.h"
#include "../../Rendering/Framebuffer.h"
#include "../../Rendering/Font.h"


#include <stdint.h>

namespace PageAllocator
{
    void Init(EFI_MEMORY_MAP* MemoryMap, Framebuffer* ScreenBuffer, PSF_FONT* PSFFont);

    void* RequestPage();

    uint64_t GetFreeMemory();

    uint64_t GetLockedMemory();

    uint64_t GetTotalMemory();

    void* LockPage(void* Address);

    void* FreePage(void* Address);

    void LockPages(void* Address, uint64_t Count);

    void FreePages(void* Address, uint64_t Count);
}