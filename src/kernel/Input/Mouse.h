#pragma once

#include "STL/Math/Point.h"

namespace Mouse
{
    extern STL::Point Position;

    void InitPS2();

    void HandleMouseData(uint8_t Data);
}