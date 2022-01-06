#include "SystemCalls.h"
#include "System.h"

namespace SystemCalls
{    
    struct InterruptFrame;

    uint64_t Args[10];

    void Call_System()
    {
        Args[1] = (uint64_t)System::System((char*)Args[1]);
    }

    void(*Calls[])() = 
    {
        Call_System
    };

    void Call()
    {
        if (Args[0] < 0 || Args[0] > sizeof(Calls)/sizeof(*Calls))
        {
            return;
        }  

        Calls[Args[0]]();
    }

}