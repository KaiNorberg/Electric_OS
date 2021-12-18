#pragma once

#include <stdint.h>

struct Point
{
    int32_t X;
    int32_t Y;

    Point() = default;
    Point(int32_t X, int32_t Y);
};