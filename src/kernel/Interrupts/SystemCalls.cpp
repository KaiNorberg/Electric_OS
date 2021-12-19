#include "SystemCalls.h"
#include "../Core/Panic.h"
#include "../String/cstr.h"
#include "../PIT/PIT.h"

namespace SystemCalls
{
    void SysCall()
    {
        static void(*SystemArray[])() = 
        {
            SysCall_Panic,
            SysCall_GetTicks
        };
    
        if (*(SYSCALL_SELECTOR) < 0 || *(SYSCALL_SELECTOR) > sizeof(SystemArray)/sizeof(*SystemArray))
        {
            KernelPanic("Invalid SYSCALL_SELECTOR value.");
        }        

        SystemArray[*(SYSCALL_SELECTOR)]();
    }

    void SysCall_Panic()
    {
        KernelPanic(*((char**)SYSCALL_ARG0));
    }

    void SysCall_GetTicks()
    {
        *((uint64_t*)SYSCALL_ARG0) = PIT::Ticks;
    }
}