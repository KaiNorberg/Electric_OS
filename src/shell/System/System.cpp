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
    
    void* GetSysCallAddr()
    {
        return SysCallAddr;
    }

    void SetSelector(uint64_t Selector)
    {
        *((uint64_t*)(SysCallAddr)) = Selector;
    }
}