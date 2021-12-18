#include "SystemCalls.h"
#include "../Core/Panic.h"
#include "../String/cstr.h"

namespace SystemCalls
{
    void SysCall()
    {
        static void(*SystemArray[])() = 
        {
            SysCall_Panic
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
}