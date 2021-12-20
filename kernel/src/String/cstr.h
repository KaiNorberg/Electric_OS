#pragma once
#include <stdint.h>

namespace cstr
{
    char IntToChar(uint8_t Number);

    char* ToString(uint64_t Number);

    char* ToString(int64_t Number);

    char* ToString(float Number, uint32_t DecimalAmount = 5);
}
