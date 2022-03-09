#pragma once

#include <stdint.h>

#include "Point.h"

namespace STL
{
    uint32_t Rand();

    uint32_t GetDigit(uint32_t Number, uint32_t Digit);

    uint32_t GetDigitAmount(uint32_t Number);

    uint32_t IntPow(uint32_t X, uint32_t E);

    double Sqrt(double X);

    bool Contains(Point TopLeft, Point BottomRight, Point Pos);

    bool Contains(Point TopLeft0, Point BottomRight0, Point TopLeft1, Point BottomRight1);

    template <typename T>
    T RoundToMultiple(const T N, const T Multiple) 
    {
        return N + N % Multiple;
    }

    template <typename T>
    T Clamp(const T& n, const T& lower, const T& upper) 
    {
        return n + ((n < lower) * (lower - n)) + ((n > upper) * (upper - n));
    }

    template <typename T> 
    T Min (const T& A, const T& B) 
    {
        return !(B < A) ? A : B;
    }

    template <typename T> 
    T Max (const T& A, const T& B) 
    {
        return !(B > A) ? A : B;
    }
}
