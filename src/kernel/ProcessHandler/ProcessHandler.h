#pragma once

#include "Process.h"

#include "STL/Process/Process.h"
#include "STL/List/List.h"

namespace ProcessHandler    
{       
    extern Process* FocusedProcess;
    extern uint64_t LastMessagedProcess;
    extern bool SwapBuffersRequest;
    extern uint64_t MovingWindow;

    extern STL::List<Process*> Processes;

    void KillAllProcesses();

    Process* GetProcess(uint64_t ID);

    void KeyBoardInterupt();

    void MouseInterupt();

    void PITInterupt();

    bool KillProcess(uint64_t ProcessID);

    uint64_t StartProcess(STL::PROC Procedure);

    void Loop();   
}