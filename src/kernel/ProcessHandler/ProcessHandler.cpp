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
    uint64_t LastMessagedProcess = 0;

    bool SwapBuffersRequest = false;

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
            uint8_t Key = KeyBoard::GetKeyPress();
            Processes[i].SendMessage(STL::PROM::KEYPRESS, &Key);
        }
    }

    void MouseInterupt()
    {
        SwapBuffersRequest = true;
    }   

    void PITInterupt()
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            uint64_t Tick = PIT::Ticks;
            Processes[i].SendMessage(STL::PROM::TICK, &Tick);
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

        if (Processes.Last().GetType() == STL::PROT::FULLSCREEN)
        {
            for (int i = 0; i < Processes.Length() - 1; i++)
            {
                if (Processes[i].GetType() == STL::PROT::FULLSCREEN)
                {
                    KillProcess(Processes[i].GetID());
                    return;
                }
            }
        }
    }

    void Loop()
    {                
        StartProcess(tty::Procedure);

        while (true) 
        {       
            for (uint64_t i = 0; i < Processes.Length(); i++)
            {
                switch (Processes[i].GetRequest())
                {
                case STL::PROR::CLEAR:
                {
                    Processes[i].Clear();
                }
                break;
                case STL::PROR::DRAW:
                {
                    Processes[i].Draw();

                    for (int j = 0; j < Processes.Length(); j++)
                    {
                        if (j == i)
                        {
                            continue;
                        }

                        if (Processes[j].GetDepth() > Processes[i].GetDepth())
                        {
                            Processes[j].Render();
                        }
                    }
                }
                break;
                case STL::PROR::KILL:
                {
                    Processes[i].Kill();
                    Processes.Erase(i);
                    i--;
                }
                break;
                default:
                {

                }
                break;
                }
            }

            if (SwapBuffersRequest)
            {
                Renderer::SwapBuffers();
                SwapBuffersRequest = false;
            }

            if (Processes.Length() == 0)
            {
                Debug::Error("All processes killed");
            }

            asm("HLT");
        }
    }
}