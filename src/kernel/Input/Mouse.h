#pragma once

#include "STL/Math/Point.h"

namespace Mouse
{
    extern STL::Point Position;

    extern bool LeftPressed;
    
    extern bool MiddlePressed;
    
    extern bool RightPressed;

    void InitPS2();

    void HandleMousePacket(uint8_t* MousePacket);
}