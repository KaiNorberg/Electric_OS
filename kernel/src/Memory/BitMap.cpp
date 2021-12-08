#include "BitMap.h"

uint64_t BitMap::Size()
{
    return this->ByteAmount;
}

uint64_t BitMap::Entries()
{
    return this->ByteAmount * 8;
}

void* BitMap::GetBuffer()
{
    return (void*)this->Buffer;
}

bool BitMap::Set(uint64_t Index, bool Value)
{
    if (Index > this->ByteAmount * 8)
    {
        return false;
    }

    uint64_t ByteIndex = Index / 8;
    uint64_t BitIndex = Index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;

    Buffer[ByteIndex] &= ~BitIndexer;
    if (Value)
    {
        Buffer[ByteIndex] |= BitIndexer;
    }

    return true;
}

bool BitMap::operator[](uint64_t Index)
{
    if (Index > this->ByteAmount * 8)
    {
        return false;
    }
    
    uint64_t ByteIndex = Index / 8;
    uint64_t BitIndex = Index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;

    return (Buffer[ByteIndex] & BitIndexer) > 0;
}

BitMap::BitMap(uint64_t Entries, void* Buffer)
{
    this->ByteAmount = Entries / 8;
    this->Buffer = (uint8_t*)Buffer;
}