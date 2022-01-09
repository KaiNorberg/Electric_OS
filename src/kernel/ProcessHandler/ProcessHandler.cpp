#include "ProcessHandler.h"

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
    uint64_t FocusedProcess = 0;
    uint64_t LastMessagedProcess = 0;

    bool RedrawMouse = false;

    Process* GetProcess(uint64_t ID)
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            if (Processes[i].GetID() == ID)
            {
                return &Processes[i];
            }
        }
        
        return nullptr;
    }

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
                RedrawMouse = false;
            }

            for (int i = 0; i < Processes.Length(); i++)
            {
                STL::PROR Request = Processes[i].GetRequest();
                if (Request != STL::PROR::SUCCESS)
                {
                    switch (Request)
                    {
                    case STL::PROR::DRAW:
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