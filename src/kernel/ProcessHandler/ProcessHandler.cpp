#include "ProcessHandler.h"

#include "STL/Math/Point.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/Memory/Memory.h"
#include "STL/String/cstr.h"

#include "kernel/Debug/Debug.h"

#include "Programs/tty/tty.h"

#include "kernel/Renderer/Renderer.h"
#include "kernel/Memory/Heap.h"
#include "kernel/Input/KeyBoard.h"
#include "kernel/Input/Mouse.h"
#include "kernel/PIT/PIT.h"

namespace ProcessHandler
{    
    STL::List<Process*> Processes;
    uint64_t LastMessagedProcess = 0;

    bool SwapBuffersRequest = false;

    void KillAllProcesses()
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            KillProcess(Processes[i]->GetID());
        }
    }

    Process* GetProcess(uint64_t ID)
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            if (Processes[i]->GetID() == ID)
            {
                return Processes[i];
            }
        }
        
        return nullptr;
    }

    void KeyBoardInterupt()
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            uint8_t Key = KeyBoard::GetKeyPress();
            Processes[i]->SendMessage(STL::PROM::KEYPRESS, &Key);
        }
    }

    void MouseInterupt()
    {        
        for (int i = 0; i < Processes.Length(); i++)
        {
            STL::MINFO MouseInfo;
            MouseInfo.Pos = Mouse::Position;
            MouseInfo.LeftHeld = Mouse::LeftHeld;
            MouseInfo.MiddleHeld = Mouse::MiddleHeld;
            MouseInfo.RightHeld = Mouse::RightHeld;

            Processes[i]->SendMessage(STL::PROM::MOUSE, &MouseInfo);
        }

        Mouse::LeftHeld = false;
        Mouse::MiddleHeld = false;
        Mouse::RightHeld = false;

        SwapBuffersRequest = true;
    }   

    void PITInterupt()
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            uint64_t Tick = PIT::Ticks;
            Processes[i]->SendMessage(STL::PROM::TICK, &Tick);
        }
    }

    bool KillProcess(uint64_t ProcessID)
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            if (ProcessID == Processes[i]->GetID())
            {
                Processes[i]->Kill();
                delete Processes[i];
                Processes.Erase(i);
                return true;
            }
        }

        return false;
    }

    uint64_t StartProcess(STL::PROC Procedure)
    {        
        Processes.Push(new Process(Procedure));

        if (Processes.Last()->GetType() == STL::PROT::FULLSCREEN)
        {
            for (int i = 0; i < Processes.Length() - 1; i++)
            {
                if (Processes[i]->GetType() == STL::PROT::FULLSCREEN)
                {
                    KillProcess(Processes[i]->GetID());
                    break;
                }
            }
        }

        return Processes.Last()->GetID();
    }

    void Loop()
    {                
        StartProcess(tty::Procedure);
    
        while (true) 
        {       
            for (uint64_t i = 0; i < Processes.Length(); i++)
            {
                switch (Processes[i]->GetRequest())
                {
                case STL::PROR::CLEAR:
                {
                    Processes[i]->Clear();
                }
                break;
                case STL::PROR::DRAW:
                {
                    Processes[i]->Draw();
                }
                break;
                case STL::PROR::KILL:
                {
                    KillProcess(Processes[i]->GetID());
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