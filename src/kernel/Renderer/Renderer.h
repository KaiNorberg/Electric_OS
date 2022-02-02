#pragma once

#include "STL/Graphics/Font.h"
#include "STL/Graphics/ARGB.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/Math/Math.h"
#include "STL/Memory/Memory.h"

namespace Renderer
{    
    extern STL::Framebuffer Backbuffer;

    extern STL::Point CursorPos;
    extern STL::Point OldMousePos;

    extern STL::ARGB Background;
    extern STL::ARGB Foreground;

    extern bool DrawMouse;

    void Init(STL::Framebuffer* framebuffer, STL::PSF_FONT * PSF_Font);

    void PutPixel(STL::Point Pixel, STL::ARGB Color);

    STL::ARGB GetPixel(STL::Point Pixel);

    void PutChar(char chr, STL::Point Pos, uint8_t Scale = 1);

    void Print(const char* str, uint8_t Scale = 1);

    void Print(char Chr, uint8_t Scale = 1);

    void ScrollUp(uint64_t Amount);

    void Clear();

    void RedrawMouse();

    void SwapBuffers();

    STL::Point GetScreenSize();
}
