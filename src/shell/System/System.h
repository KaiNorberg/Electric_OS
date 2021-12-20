#pragma once

#include <stdint.h>

#include "../Math/Point.h"

#define SYSCALL_INT 0x80

#define SYSCALL_PANIC_INDEX 0
#define SYSCALL_GETTICKS_INDEX 1
#define SYSCALL_GETMOUSEPOS_INDEX 2

namespace System
{
    void Init(void* Address);

    void Panic(char* Message);

    void Sleep(double Seconds);

    Point GetMousePos();

    void* GetSysCallAddr();

    void SetSelector(uint64_t Selector);

    void* GetReturn(uint64_t Index);

    template<typename T>
    void SetArg(T Arg, uint64_t Index)
    {
        *((T*)(((uint64_t)GetSysCallAddr()) + 0x08 * (Index + 1))) = Arg;
    }
}