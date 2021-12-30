#pragma once
#include <stdint.h>

namespace cstr
{
    char* ToString(uint64_t Number);

    uint64_t HashWord(const char* String);

    uint64_t Hash(const char* String);
}
