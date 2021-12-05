#pragma once

#include <stdint.h>

struct PageMapIndexer
{
    uint64_t PDPindex;
    uint64_t PDindex;
    uint64_t PTindex;
    uint64_t Pindex;

    PageMapIndexer(uint64_t VirtualAddress);
};
