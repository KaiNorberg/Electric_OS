#include "System.h"

#include "kernel/System/System.h"

void* operator new(uint64_t Size) 
{
    return STL::Malloc(Size);
}

void operator delete(void* Address, uint64_t Size) 
{
    return STL::Free(Address);
}

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