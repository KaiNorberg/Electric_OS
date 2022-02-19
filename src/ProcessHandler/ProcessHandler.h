#pragma once

#include "Process.h"

#include "STL/Process/Process.h"
#include "STL/List/List.h"

namespace ProcessHandler    
{           
    extern Process* FocusedProcess;
    extern Process* LastMessagedProcess;
    extern Process* MovingWindow;

    extern STL::List<Process*> Processes;

    void KeyBoardInterupt();

    void MouseInterupt();

    void PITInterupt();

    void KillAllProcesses();

    Process* GetProcess(uint64_t ID);

    bool KillProcess(uint64_t ProcessID);

    uint64_t StartProcess(STL::PROC Procedure);

    void Loop();   
}