#include "BitMap.h"

uint64_t BitMap::Size()
{
    return m_Size;
}

bool BitMap::operator[](uint64_t Index)
{
    uint64_t ByteIndex = Index / 8;
    uint64_t BitIndex = Index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;

    return (m_Buffer[ByteIndex] & BitIndexer) > 0;
}

void BitMap::Set(uint64_t Index, bool Value)
{
    uint64_t ByteIndex = Index / 8;
    uint64_t BitIndex = Index % 8;
    uint8_t BitIndexer = 0b10000000 >> BitIndex;

    m_Buffer[ByteIndex] &= ~BitIndexer;
    if (Value)
    {
        m_Buffer[ByteIndex] |= BitIndexer;
    }
}

BitMap::BitMap(uint64_t Size, void* Buffer)
{
    this->m_Size = Size;
    this->m_Buffer = (uint8_t*)Buffer;
}
