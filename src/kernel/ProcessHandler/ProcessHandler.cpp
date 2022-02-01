#include "ProcessHandler.h"

#include "STL/Math/Math.h"
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

    Process* FocusedProcess = nullptr;

    bool SwapBuffersRequest = false;

    uint64_t MovingWindow = 0;
    STL::Point MovingWindowPosDelta = STL::Point(0, 0);

    void KillAllProcesses()
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            Processes[i]->Kill();
            delete Processes[i];
        }
        Processes.Clear();
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
        if (FocusedProcess == nullptr)
        {
            return;
        }

        uint8_t Key = KeyBoard::GetKeyPress();
        FocusedProcess->SendMessage(STL::PROM::KEYPRESS, &Key);
    }

    void MouseInterupt()
    {        
        if (MovingWindow != 0)
        {
            if (!Mouse::LeftHeld)
            {       
                for (int i = 0; i < MovingWindow; i++)
                {
                    if (Processes[MovingWindow]->Contains(Processes[i]));
                    {
                        Processes[i]->SendRequest(STL::PROR::RENDER);
                    }
                }

                FocusedProcess = Processes[MovingWindow];
                Processes[MovingWindow]->SetPos(Mouse::Position + MovingWindowPosDelta);
                Processes[MovingWindow]->SendRequest(STL::PROR::DRAW);

                MovingWindow = 0;
            }       
        }

        for (int i = Processes.Length(); i --> 0; )
        {                
            STL::MINFO MouseInfo;
            MouseInfo.Pos = Mouse::Position - Processes[i]->GetPos();
            MouseInfo.LeftHeld = Mouse::LeftHeld;
            MouseInfo.MiddleHeld = Mouse::MiddleHeld;
            MouseInfo.RightHeld = Mouse::RightHeld;

            Processes[i]->SendMessage(STL::PROM::MOUSE, &MouseInfo); 

            if (Mouse::LeftHeld)
            {                
                if (Processes[i]->GetType() == STL::PROT::WINDOWED)
                {
                    STL::Point CloseButtonPos = Processes[i]->GetPos() + STL::Point(Processes[i]->GetSize().X, 0) + CLOSE_BUTTON_OFFSET;

                    if (STL::Contains(CloseButtonPos, CloseButtonPos + CLOSE_BUTTON_SIZE, Mouse::Position)) //If over close button
                    {
                        KillProcess(Processes[i]->GetID());
                        break;
                    } 
                    else if (STL::Contains(Processes[i]->GetPos() - FRAME_OFFSET, Processes[i]->GetPos() + STL::Point(Processes[i]->GetSize().X, 0), Mouse::Position)) //If over topbar
                    {                              
                        FocusedProcess = Processes[i];
                        MovingWindow = i;
                        MovingWindowPosDelta = Processes[i]->GetPos() - Mouse::Position;

                        break;
                    }                    
                }
               
                if (Processes[i]->Contains(Mouse::Position)) //If over window
                {
                    if (Processes[i] != FocusedProcess)
                    {
                        FocusedProcess = Processes[i];
                        if (Processes[i]->GetType() == STL::PROT::WINDOWED)
                        {
                            Processes[i]->SendRequest(STL::PROR::RENDER);
                        }                        
                    } 

                    break;
                }
            }
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
                if (Processes[i] == FocusedProcess)
                {
                    FocusedProcess = nullptr;
                }

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
        FocusedProcess = Processes[0];

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
                case STL::PROR::RENDER:
                {
                    Processes[i]->Render();
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
                case STL::PROR::RESET:
                {
                    KillAllProcesses();
                    Loop();
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
                StartProcess(tty::Procedure);
            }

            asm("HLT");
        }
    }
}