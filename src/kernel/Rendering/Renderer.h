#pragma once

#include "Font.h"
#include "ARGB.h"
#include "Framebuffer.h"
#include "../Math/Math.h"
#include "../Memory/Memory.h"

namespace Renderer
{
    void Init(Framebuffer* framebuffer, PSF1_FONT* PSF1_Font);

    void PutChar(char chr, ARGB const& Color, uint64_t X, uint64_t Y);

    void Print(const char* str, ARGB Color = ARGB(255));

    void Print(char Chr, ARGB Color = ARGB(255));

    void Clear(ARGB Color = ARGB(0));

    void GetScreenSize(uint64_t* X, uint64_t* Y);
}
