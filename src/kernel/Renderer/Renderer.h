#pragma once

#include "Font.h"
#include "ARGB.h"
#include "Framebuffer.h"
#include "../Math/Math.h"
#include "../Memory/Memory.h"

namespace Renderer
{
    extern Point CursorPos;

    extern ARGB Background;
    extern ARGB Foreground;

    extern bool DrawMouse;

    void Init(Framebuffer* framebuffer, PSF_FONT* PSF_Font);

    void PutPixel(Point Pixel, ARGB Color);

    void PutPixelFront(Point Pixel, ARGB Color);

    ARGB GetPixel(Point Pixel);

    ARGB GetPixelFront(Point Pixel);

    void PutChar(char chr, Point Pos, uint8_t Scale = 1);

    void Print(const char* str, uint8_t Scale = 1);

    void Print(char Chr, uint8_t Scale = 1);

    void ScrollUp(uint64_t Amount);

    void Clear(ARGB Color = ARGB(0));

    void SwapBuffers();

    Point GetScreenSize();
}
