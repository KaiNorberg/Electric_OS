#pragma once

#include <stddef.h>
#include <stdint.h>

class BitMap
{
public:

    uint64_t Size();

    uint64_t Entries();

    void* GetBuffer();

    bool Set(uint64_t Index, bool Value);

    bool operator[](uint64_t Index);

    BitMap() = default;

    BitMap(uint64_t Entries, void* Buffer);

private:
    uint8_t* Buffer = nullptr;
    uint64_t ByteAmount = 0;
};
