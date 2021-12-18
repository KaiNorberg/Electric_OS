#pragma once

#include <stdint.h>

namespace Mouse
{
    extern uint64_t XPos;
    extern uint64_t YPos;

    void InitPS2();

    void HandleMouseData(uint8_t Data);
}