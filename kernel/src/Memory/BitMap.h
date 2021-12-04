#pragma once

#include <stddef.h>
#include <stdint.h>

class BitMap
{
public:

    uint64_t Size();

    uint64_t Entries();

    void* GetBuffer();

    bool operator[](uint64_t Index);

    void Set(uint64_t Index, bool Value);

    BitMap() = default;

    BitMap(uint64_t Entries, void* Buffer);

private:
    uint8_t* Buffer = nullptr;
    uint64_t ByteAmount = 0;
};
