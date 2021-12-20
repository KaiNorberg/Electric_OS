#include "System.h"

namespace System
{
    void* SysCallAddr = nullptr;

    void Init(void* Address)
    {
        bool Initialized = false;
        if (Initialized)
        {
            return;
        }
        Initialized = true;

        SysCallAddr = Address;     
    }

    void Panic(char* Message)
    {
        SetSelector(SYSCALL_PANIC_INDEX);
        SetArg(Message, 0);
        asm("INT $0x80");
    }

    void Sleep(double Seconds)
    {
        System::SetSelector(SYSCALL_GETTICKS_INDEX);
        asm("INT $0x80");
        uint64_t StartTicks = *(uint64_t*)GetReturn(0);
        while (*(uint64_t*)GetReturn(0) < StartTicks + Seconds * 57.0)
        {
            asm("HLT");
            asm("INT $0x80");
        }
    }

    Point GetMousePos()
    {
        System::SetSelector(SYSCALL_GETMOUSEPOS_INDEX);
        asm("INT $0x80");

        return Point(*((int64_t*)GetReturn(0)), *((int64_t*)GetReturn(1)));
    }

    void* GetSysCallAddr()
    {
        return SysCallAddr;
    }

    void* GetReturn(uint64_t Index)
    {
        return (void*)(((uint64_t)SysCallAddr) + 0x08 * (Index + 1));
    }

    void SetSelector(uint64_t Selector)
    {
        *((uint64_t*)(SysCallAddr)) = Selector;
    }
}