#include "SystemCalls.h"
#include "../Core/Panic.h"
#include "../String/cstr.h"

namespace SystemInterupts
{
    __attribute__((interrupt)) void Panic(InterruptFrame* frame)
    {		
        KernelPanic(*(SYSCALL_PANIC_ARG0));
    }
}