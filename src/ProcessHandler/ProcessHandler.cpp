#include "ProcessHandler.h"
#include "Compositor.h"

#include "STL/Math/Math.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/Memory/Memory.h"
#include "STL/String/cstr.h"
#include "STL/GUI/Button.h"

#include "Debug/Debug.h"

#include "Programs/tty/tty.h"

#include "Renderer/Renderer.h"
#include "Memory/Heap.h"
#include "Input/KeyBoard.h"
#include "Input/Mouse.h"
#include "PIT/PIT.h"

namespace ProcessHandler
{        
    STL::List<Process*> Processes;

    Process* LastMessagedProcess = nullptr;
    Process* FocusedProcess = nullptr;
    Process* MovingWindow = nullptr;

    STL::Point MovingWindowPosDelta = STL::Point(0, 0);

    void SetFocusedProcess(Process* NewFocus)
    {
        if (NewFocus == nullptr)
        {
            FocusedProcess = nullptr;
            return;
        }

        if (FocusedProcess != nullptr && FocusedProcess->GetType() == STL::PROT::WINDOWED)
        {                       
            Compositor::RedrawRequest = true;
        }

        if (NewFocus->GetType() == STL::PROT::WINDOWED)
        {                       
            Compositor::RedrawRequest = true;
        }

        FocusedProcess = NewFocus;       

        if (NewFocus->GetType() != STL::PROT::FULLSCREEN)
        {
            FocusedProcess->SetDepth(Processes.Length() - 1);
        }  
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
        if (MovingWindow != nullptr)
        {                
            MovingWindow->SetPos(Mouse::Position + MovingWindowPosDelta);

            if (!Mouse::LeftHeld)
            {       
                FocusedProcess = MovingWindow;
                MovingWindow = 0;
            }       
            Compositor::RedrawRequest = true;
        }
        else
        {
            for (uint32_t i = Processes.Length(); i --> 0; )
            {                    
                if (Mouse::LeftHeld && (Processes[i]->GetType() == STL::PROT::WINDOWED || Processes[i]->GetType() == STL::PROT::MINIMIZED) && 
                    STL::Contains(Processes[i]->GetPos() - FRAME_OFFSET, Processes[i]->GetPos() + STL::Point(Processes[i]->GetSize().X, 0), Mouse::Position))
                {               
                    STL::Point CloseButtonPos = Processes[i]->GetCloseButtonPos();
                    if (STL::Contains(CloseButtonPos, CloseButtonPos + CLOSE_BUTTON_SIZE, Mouse::Position)) //If over close button
                    {
                        KillProcess(Processes[i]->GetID());
                        break;
                    }

                    STL::Point MinimizeButtonPos = Processes[i]->GetMinimizeButtonPos();
                    if (STL::Contains(MinimizeButtonPos, MinimizeButtonPos + MINIMIZE_BUTTON_SIZE, Mouse::Position)) //If over close button
                    {
                        Processes[i]->Minimize();
                        Compositor::RedrawRequest = true;
                        break;
                    }

                    MovingWindow = Processes[i];
                    MovingWindowPosDelta = Processes[i]->GetPos() - Mouse::Position;

                    SetFocusedProcess(Processes[i]);    
                    break;
                }  
                else if (Processes[i]->Contains(Mouse::Position)) //If over window
                {
                    STL::MINFO MouseInfo;
                    MouseInfo.Pos = Mouse::Position - Processes[i]->GetPos();
                    MouseInfo.LeftHeld = Mouse::LeftHeld;
                    MouseInfo.MiddleHeld = Mouse::MiddleHeld;
                    MouseInfo.RightHeld = Mouse::RightHeld;

                    Processes[i]->SendMessage(STL::PROM::MOUSE, &MouseInfo);     

                    if (Mouse::LeftHeld)
                    {
                        SetFocusedProcess(Processes[i]);    
                    } 

                    break;
                }
            }
        }

        Mouse::LeftHeld = false;
        Mouse::MiddleHeld = false;
        Mouse::RightHeld = false;

        Renderer::RedrawMouse();
    }   

    void PITInterupt()
    {
        for (uint32_t i = 0; i < Processes.Length(); i++)
        {
            uint64_t Tick = PIT::Ticks;
            Processes[i]->SendMessage(STL::PROM::TICK, &Tick);
        }
    }

    void KillAllProcesses()
    {
        for (uint32_t i = 0; i < Processes.Length(); i++)
        {
            Processes[i]->Kill();
            delete Processes[i];
        }
        Processes.Clear();
    }

    Process* GetProcess(uint64_t ID)
    {
        for (uint32_t i = 0; i < Processes.Length(); i++)
        {
            if (Processes[i]->GetID() == ID)
            {
                return Processes[i];
            }
        }
        
        return nullptr;
    }

    bool KillProcess(uint64_t ProcessID)
    {
        for (uint32_t i = 0; i < Processes.Length(); i++)
        {
            if (ProcessID == Processes[i]->GetID())
            {
                if (Processes[i] == FocusedProcess)
                {
                    FocusedProcess = nullptr;
                }

                if (Processes[i] == LastMessagedProcess)
                {
                    LastMessagedProcess = nullptr;
                }

                if (Processes[i] == MovingWindow)
                {
                    MovingWindow = nullptr;
                }

                Processes[i]->Kill();
                delete Processes[i];
                Processes.Erase(i);
                Compositor::RedrawRequest = true;
                return true;
            }
        }

        return false;
    }

    uint64_t StartProcess(STL::PROC Procedure)
    {                    
        for (uint32_t i = 0; i < Processes.Length(); i++)
        {
            if (Processes[i]->GetProcedure() == Procedure)
            {
                return 0;
            }
        }

        Process* NewProcess = new Process(Procedure);

        if (NewProcess->GetType() == STL::PROT::FULLSCREEN)
        {
            for (uint32_t i = 0; i < Processes.Length(); i++)
            {
                if (Processes[i]->GetType() == STL::PROT::FULLSCREEN)
                {
                    KillProcess(Processes[i]->GetID());
                    break;
                }
            }
        }

        Processes.Push(NewProcess);
        NewProcess->UpdateDepth();

        return NewProcess->GetID();
    }

    void Loop()
    {                
        StartProcess(tty::Procedure);
        FocusedProcess = Processes[0];
        
        while (true) 
        {   
            for (uint32_t i = 0; i < Processes.Length(); i++)
            {
                switch (Processes[i]->PopRequest())
                {
                case STL::PROR::CLEAR:
                {
                    Processes[i]->Clear();
                    Compositor::Update(i);
                }
                break;
                case STL::PROR::DRAW:
                {
                    Processes[i]->Draw();
                    Compositor::Update(i);
                }
                break;
                case STL::PROR::KILL:
                {
                    KillProcess(Processes[i]->GetID());
                    i--;
                    Compositor::RedrawRequest = true;
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

            Compositor::Update();

            if (Processes.Length() == 0)
            {
                StartProcess(tty::Procedure);
                FocusedProcess = Processes[0];
            }

            asm("HLT");
        }
    }
}