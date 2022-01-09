#pragma once

#include "STL/Process/Process.h"

namespace ProcessHandler
{
    void KeyBoardInterupt();

    void MouseInterupt();

    void PITInterupt();

    bool KillProcess(uint64_t ProcessID);

    void StartProcess(STL::PROC Procedure);

    void Loop();   
}