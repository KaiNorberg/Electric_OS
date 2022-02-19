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
    extern STL::ARGB Background;
    extern STL::ARGB Foreground;

    extern bool DrawMouse;
    extern STL::Point OldMousePos;

    void Init(STL::Framebuffer* Screenbuffer);

    void Print(const char* str, uint8_t Scale = 1);

    void Print(char Chr, uint8_t Scale = 1);

    void RedrawMouse();

    void SwapBuffers();

    STL::Point GetScreenSize();
}
