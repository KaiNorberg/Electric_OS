#pragma once

#include <stdint.h>

namespace STL
{ 
    struct Point
    {
        int32_t X;
        int32_t Y;

        Point() = default;
        Point(int32_t X, int32_t Y);
    };  
}