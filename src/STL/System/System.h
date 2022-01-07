#pragma once

#include <stdint.h>

#define SYSCALL_SYSTEM 0

namespace STL
{
    typedef uint64_t SYSRV;

    const char* System(const char* Command);

    void* Malloc(uint64_t Size);

    void Free(void* Memory);
}