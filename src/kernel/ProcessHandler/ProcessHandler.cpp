#include "ProcessHandler.h"
#include "Process.h"

#include "STL/Math/Point.h"
#include "STL/List/List.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/Memory/Memory.h"
#include "kernel/Debug/Debug.h"

#include "Programs/tty/tty.h"

#include "kernel/Renderer/Renderer.h"
#include "kernel/Memory/Heap.h"
#include "kernel/Input/KeyBoard.h"
#include "kernel/Input/Mouse.h"
#include "kernel/PIT/PIT.h"

namespace ProcessHandler
{    
    STL::List<Process> Processes;
    int8_t FocusedProcess = -1;

    volatile bool RedrawMouse = false;

    void KeyBoardInterupt()
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            if (Processes[i].GetID() == FocusedProcess || Processes[i].GetType() == STL::PROT::BACKGROUND)
            {
                uint8_t Key = KeyBoard::GetKeyPress();
                Processes[i].SendMessage(STL::PROM::KEYPRESS, &Key);
            }
        }
    }

    void MouseInterupt()
    {
        RedrawMouse = true;
    }   

    void PITInterupt()
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            if (Processes[i].GetID() == FocusedProcess || Processes[i].GetType() == STL::PROT::BACKGROUND)
            {
                uint64_t Tick = PIT::Ticks;
                Processes[i].SendMessage(STL::PROM::TICK, &Tick);
            }
        }
    }

    void SendMessage(STL::PROM Message, STL::PROI Input)
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            Processes[i].SendMessage(Message, Input);
        }
    }

    bool KillProcess(uint64_t ProcessID)
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            if (ProcessID == Processes[i].GetID())
            {
                Processes[i].Kill();
                Processes.Erase(i);
                return true;
            }
        }

        return false;
    }

    void StartProcess(STL::PROC Procedure)
    {
        Processes.Push(Process(Procedure));
        FocusedProcess = Processes.Last().GetID();
    }

    void Loop()
    {
        StartProcess(tty::Procedure);

        while (true)
        {       
            if (RedrawMouse)
            {
                Renderer::RedrawMouse();
            }

            for (int i = 0; i < Processes.Length(); i++)
            {
                if (Processes[i].GetRequest() != STL::PROR::SUCCESS)
                {
                    switch (Processes[i].GetRequest())
                    {
                    case STL::PROR::REDRAW:
                    {
                        Processes[i].Draw();
                    }
                    break;
                    case STL::PROR::KILL:
                    {
                        Processes[i].Kill();
                        Processes.Erase(i);
                    }
                    break;
                    }
                }
            }

            if (Processes.Length() == 0)
            {
                Debug::Error("All processes killed");
            }

            asm("HLT");
        }
    }
}