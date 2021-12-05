#pragma once

#include <stdint.h>

enum PageTableFlag
{
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63 // only if supported
};

class PageDirEntry
{
public:

    void SetFlag(PageTableFlag Flag, bool Enabled);

    bool GetFlag(PageTableFlag Flag);

    void SetAddress(uint64_t Address);

    uint64_t GetAddress();

private:
    uint64_t Value;
};


struct PageTable
{
    PageDirEntry Entries[512];
}__attribute__((aligned(0x1000)));
