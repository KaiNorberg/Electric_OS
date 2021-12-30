#pragma once
#include <stdint.h>

namespace cstr
{
    char* ToString(uint64_t Number);

    uint64_t ToInt(const char* String);

    char* NextWord(const char* String);

    uint64_t HashWord(const char* String);

    uint64_t Length(const char* String);

    char* Copy(char* Dest, const char* Source);
}
