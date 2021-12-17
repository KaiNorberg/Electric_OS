#pragma once

#include <stdint.h>

extern uint64_t _KernelStart;

#define SYSCALL_PANIC_INT 0xFF
#define SYSCALL_PANIC_ARG0 (char**)(((uint64_t)&_KernelStart) + 0x01)

namespace SystemInterupts
{
    struct InterruptFrame;

    __attribute__((interrupt)) void Panic(InterruptFrame* frame); //0xFF
}