#pragma once

#include <stdint.h>

#include "STL/Graphics/Framebuffer.h"

namespace Compositor
{
    extern bool RedrawRequest;
    extern bool BufferSwapRequest;
    extern bool DrawMouse;

    void Init(STL::Framebuffer* Screenbuffer);

    void RedrawMouse();

    void Update(uint32_t i);

    void Update();
}