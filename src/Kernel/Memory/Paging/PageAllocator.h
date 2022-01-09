#pragma once

#include "STL/Graphics/Framebuffer.h"
#include "STL/Graphics/Font.h"

#include "kernel/Memory/efiMemory.h"

#include <stdint.h>

namespace PageAllocator
{    
    extern uint64_t PageAmount;

    void Init(EFI_MEMORY_MAP* MemoryMap, STL::Framebuffer* ScreenBuffer, STL::PSF_FONT * PSFFont);

    void* RequestPage();

    uint64_t GetFreeMemory();

    uint64_t GetLockedMemory();

    uint64_t GetTotalMemory();

    void* LockPage(void* Address);

    void* FreePage(void* Address);

    void LockPages(void* Address, uint64_t Count);

    void FreePages(void* Address, uint64_t Count);
}