#pragma once

#include <stdint.h>

namespace Math
{
    double Sqrt(double X);

    template <typename T, typename U, typename V>
    T Clamp(T n, U lower, V upper) 
    {
        return n + ((n < lower) * (lower - n)) + ((n > upper) * (upper - n));
    }
}
