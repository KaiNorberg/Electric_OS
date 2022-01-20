#pragma once

#include <stdint.h>

#define SYSCALL_SYSTEM 0

#define OS_VERSION "Electric_OS 20/01/2022"

#define ENTER 0x1C
#define BACKSPACE 0x0E
#define CONTROL 0x1D
#define LEFT_SHIFT 0x2A
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT 0x4B
#define ARROW_RIGHT 0x4D
#define PAGE_UP 0x49
#define PAGE_DOWN 0x51

namespace STL
{
    typedef uint64_t SYSRV;

    const char* System(const char* Command);

    void* Malloc(uint64_t Size);

    void Free(void* Memory);
}