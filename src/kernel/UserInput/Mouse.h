#pragma once

#include "../Math/Point.h"

namespace Mouse
{
    extern Point Position;

    void InitPS2();

    void HandleMouseData(uint8_t Data);
}