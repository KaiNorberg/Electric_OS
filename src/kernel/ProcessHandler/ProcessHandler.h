#pragma once

#include "Process.h"

#include "STL/Process/Process.h"

namespace ProcessHandler    
{   
    extern uint64_t LastMessagedProcess;
    extern bool RedrawMouse;

    Process* GetProcess(uint64_t ID);

    void KeyBoardInterupt();

    void MouseInterupt();

    void PITInterupt();

    bool KillProcess(uint64_t ProcessID);

    void StartProcess(STL::PROC Procedure);

    void Loop();   
}