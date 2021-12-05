#include "Paging.h"
#include <stdint.h>

void PageDirEntry::SetFlag(PageTableFlag Flag, bool Enabled)
{
    uint64_t BitSelector = (uint64_t)1 << Flag;
    Value &= ~BitSelector;
    if (Enabled)
    {
        Value |= BitSelector;
    }
}

bool PageDirEntry::GetFlag(PageTableFlag Flag)
{
    uint64_t BitSelector = (uint64_t)1 << Flag;
    return Value & BitSelector > 0 ? true : false;
}

uint64_t PageDirEntry::GetAddress()
{
    return (Value & 0x000ffffffffff000) >> 12;
}

void PageDirEntry::SetAddress(uint64_t Address)
{
    Address &= 0x000000ffffffffff;
    Value &= 0xfff0000000000fff;
    Value |= (Address << 12);
}
