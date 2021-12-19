#pragma once

#include "../Rendering/Framebuffer.h"
#include "../Rendering/RGBA.h"
#include "../Math/Point.h"

namespace Desktop
{
    void Init(Framebuffer* Buffer);

    void MainLoop();

    void Clear(RGBA Color = RGBA(0, 0, 0, 255));
}