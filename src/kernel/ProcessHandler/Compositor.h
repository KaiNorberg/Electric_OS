#pragma once

#include <stdint.h>

namespace Compositor
{
    extern bool RedrawRequest;
    extern bool BufferSwapRequest;

    void Update(uint32_t i);

    void Update();
}