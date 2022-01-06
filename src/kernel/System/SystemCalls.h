#pragma once

#include <stdint.h>

#define SYSCALL_INT 0x80
#define SYSCALL_SYSTEM 0x00

namespace SystemCalls
{
    extern uint64_t Args[10];

    void Call();
}