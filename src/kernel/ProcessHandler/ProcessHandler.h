#pragma once

#include "STL/Process/Process.h"

namespace ProcessHandler
{
    void KeyBoardInterupt();

    void MouseInterupt();

    void PITInterupt();

    void StartProcess(STL::PROC Procedure);

    void Loop();   
}