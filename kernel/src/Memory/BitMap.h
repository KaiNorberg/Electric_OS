#pragma once

#include <stddef.h>
#include <stdint.h>

class BitMap
{
public:

    uint64_t Size();

    bool operator[](uint64_t Index);

    void Set(uint64_t Index, bool Value);

    BitMap() = default;

    BitMap(uint64_t Size, void* Buffer);

private:
    uint8_t* m_Buffer = nullptr;
    uint64_t m_Size = 0;
};
