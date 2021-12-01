#pragma once

#include <stdint.h>

struct EFI_MEMORY_DESCRIPTOR
{
    uint32_t Type;
    void* PhysicalStart;
    void* VirtualStart;
    uint64_t NumberOfPages;
    uint64_t Attribute;
};

extern const char* EFI_MEMORY_TYPE_STRINGS[];
