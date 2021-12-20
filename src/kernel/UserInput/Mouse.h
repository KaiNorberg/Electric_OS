#pragma once

#include <stdint.h>

namespace Mouse
{
    extern int64_t XPos;
    extern int64_t YPos;

    void InitPS2();

    void HandleMouseData(uint8_t Data);
}