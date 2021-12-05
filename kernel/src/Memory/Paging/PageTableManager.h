#pragma once

#include "Paging.h"
#include "../../Rendering/Framebuffer.h"

namespace PageTableManager
{
    void MapMemory(void* VirtualMemory, void* PhysicalMemory);

    void Init(Framebuffer* ScreenBuffer);
};
