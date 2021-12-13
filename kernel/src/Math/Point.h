#pragma once

#include <stdint.h>

struct Point
{
    uint32_t X;
    uint32_t Y;

    Point() = default;
    Point(uint32_t X, uint32_t Y);
};