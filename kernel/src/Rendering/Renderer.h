#pragma once

#include "Font.h"
#include "Framebuffer.h"
#include "../Math/Math.h"
#include "../Memory/Memory.h"

namespace Renderer
{
    void Init(Framebuffer* framebuffer, PSF1_FONT* PSF1_Font);

    void PutChar(char chr, uint32_t Color, Point Pos);

    void Print(const char* str, uint32_t Color = 0xFFFFFFFF);

    void Clear(uint32_t Color = 0x000000);
}
