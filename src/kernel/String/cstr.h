#pragma once
#include <stdint.h>

#define FOREGROUND_COLOR(r, g, b) "\033F"#r#g#b
#define BACKGROUND_COLOR(r, g, b) "\033B"#r#g#b
#define NEWLINE "\n\r"

namespace cstr
{
    char* ToString(uint64_t Number);

    uint64_t ToInt(const char* String);

    char* NextWord(const char* String);

    uint64_t LineAmount(const char* String);

    uint64_t HashWord(const char* String);

    uint64_t Length(const char* String);

    char* Copy(char* Dest, const char* Source);
}
