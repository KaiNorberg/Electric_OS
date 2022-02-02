#pragma once

#include "Process.h"

#include "STL/Process/Process.h"
#include "STL/List/List.h"

namespace ProcessHandler    
{       
    extern Process* FocusedProcess;
    extern Process* LastMessagedProcess;
    extern bool SwapBuffersRequest;
    extern Process* MovingWindow;

    extern STL::List<Process*> Processes;

    void KillAllProcesses();

    Process* GetProcess(uint32_t ID);

    void KeyBoardInterupt();

    void MouseInterupt();

    void PITInterupt();

    bool KillProcess(uint32_t ProcessID);

    uint32_t StartProcess(STL::PROC Procedure);

    void Loop();   
}