#pragma once

#include "STL/Math/Point.h"

namespace Mouse
{
    extern STL::Point Position;

    extern bool LeftPressed;
    
    extern bool MiddlePressed;
    
    extern bool RightPressed;

    void InitPS2();

    /// <summary>
    /// Takes in a Mouse Packet which is defined as an array of size 4 containing unsigned bytes.
    /// </summary>
    void HandleMousePacket(uint8_t* MousePacket);
}