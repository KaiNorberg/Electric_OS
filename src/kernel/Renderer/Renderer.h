#pragma once

#include "Font.h"
#include "ARGB.h"
#include "Framebuffer.h"
#include "../Math/Math.h"
#include "../Memory/Memory.h"

namespace Renderer
{
    extern Point CursorPos;

    void Init(Framebuffer* framebuffer, PSF_FONT* PSF_Font);

    void PutPixel(Point Pixel, ARGB Color);

    ARGB GetPixel(Point Pixel);

    void PutChar(char chr, ARGB Background, ARGB Foreground, Point Pos, uint8_t Scale = 1);

    void Print(const char* str, ARGB Background = ARGB(0), ARGB Foreground = ARGB(255), uint8_t Scale = 1);

    void Print(char Chr, ARGB Background = ARGB(0), ARGB Foreground = ARGB(255), uint8_t Scale = 1);

    void ScrollUp(uint64_t Amount);

    void Clear(ARGB Color = ARGB(0));

    void SwapBuffers();

    Point GetScreenSize();
}
