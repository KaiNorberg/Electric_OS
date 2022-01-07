#include "System.h"

#include "kernel/System/System.h"

namespace STL
{    
    const char* System(const char* Command)
    {
        return (const char*)System::Call(0, Command);
    }

    void* Malloc(uint64_t Size)
    {
        return (void*)System::Call(1, Size);
    }

    void Free(void* Memory)
    {
        System::Call(2, Memory);
    }
}