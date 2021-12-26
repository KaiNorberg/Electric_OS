#pragma once

#include "Font.h"
#include "ARGB.h"
#include "Framebuffer.h"
#include "../Math/Math.h"
#include "../Memory/Memory.h"

namespace Renderer
{
    void Init(Framebuffer* framebuffer, PSF_FONT* PSF_Font);

    void PutPixel(Point Pixel, ARGB Color);

    void PutChar(char chr, ARGB Color, Point Pos);

    void Print(const char* str, ARGB Color = ARGB(255));

    void Print(char Chr, ARGB Color = ARGB(255));

    void Clear(ARGB Color = ARGB(0));

    void SwapBuffers();

    Point GetScreenSize();
}
