#pragma once

#include "STL/Math/Point.h"

namespace Mouse
{
    extern STL::Point Position;

    void InitPS2();

    void HandleMousePacket(uint8_t* MousePacket);
}