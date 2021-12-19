#pragma once

#include <stdint.h>

namespace Math
{
    uint32_t GetDigit(uint32_t Number, uint32_t Digit);

    uint32_t GetDigitAmount(uint32_t Number);

    uint32_t IntPow(uint32_t X, uint32_t E);

    template <typename T>
    T Clamp(const T& n, const T& lower, const T& upper) 
    {
        return n + ((n < lower) * (lower - n)) + ((n > upper) * (upper - n));
    }
}