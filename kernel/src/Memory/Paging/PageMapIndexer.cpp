#include "PageMapIndexer.h"

PageMapIndexer::PageMapIndexer(uint64_t VirtualAddress)
{
    VirtualAddress >>= 12;
    Pindex = VirtualAddress & 0x1ff;
    VirtualAddress >>= 9;
    PTindex = VirtualAddress & 0x1ff;
    VirtualAddress >>= 9;
    PDindex = VirtualAddress & 0x1ff;
    VirtualAddress >>= 9;
    PDPindex = VirtualAddress & 0x1ff;
}
