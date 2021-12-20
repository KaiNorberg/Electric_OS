#pragma once

#include <stdint.h>

extern uint64_t _SYS_CALL_ADDR;

#define SYSCALL_INT 0x80
#define SYSCALL_SELECTOR (uint64_t*)(((uint64_t)&_SYS_CALL_ADDR) + 0x08 * 0)
#define SYSCALL_ARG0     (void*)(((uint64_t)&_SYS_CALL_ADDR) + 0x08 * 1)
#define SYSCALL_ARG1     (void*)(((uint64_t)&_SYS_CALL_ADDR) + 0x08 * 2)
#define SYSCALL_ARG2     (void*)(((uint64_t)&_SYS_CALL_ADDR) + 0x08 * 3)
#define SYSCALL_ARG3     (void*)(((uint64_t)&_SYS_CALL_ADDR) + 0x08 * 4)
#define SYSCALL_ARG4     (void*)(((uint64_t)&_SYS_CALL_ADDR) + 0x08 * 5)

namespace SystemCalls
{
    void SysCall();

    void SysCall_Panic();

    void SysCall_GetTicks();

    void SysCall_GetMousePos();
}