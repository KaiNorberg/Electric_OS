#pragma once

#include "../Math/Point.h"

namespace Mouse
{
    void InitPS2();

    void HandleMouseData(uint8_t Data);

    Point GetPosition();
}