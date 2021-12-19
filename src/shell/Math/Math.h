#pragma once

#include <stdint.h>

namespace Math
{
    double Sqrt(double X);

    template <typename T>
    T Clamp(const T& n, const T& lower, const T& upper) 
    {
        return n + ((n < lower) * (lower - n)) + ((n > upper) * (upper - n));
    }
}
