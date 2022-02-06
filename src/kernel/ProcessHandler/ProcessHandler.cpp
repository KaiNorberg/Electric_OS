#include "ProcessHandler.h"

#include "STL/Math/Math.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/Memory/Memory.h"
#include "STL/String/cstr.h"
#include "STL/GUI/Button.h"

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
    Process* LastMessagedProcess = nullptr;

    Process* FocusedProcess = nullptr;

    Process* MovingWindow = 0;
    STL::Point MovingWindowPosDelta = STL::Point(0, 0);

    bool RedrawRequest = false;
    bool BufferSwapRequest = false;

    void SetFocusedProcess(Process* NewFocus)
    {
        if (NewFocus == nullptr)
        {
            FocusedProcess = nullptr;
            return;
        }

        if (FocusedProcess != nullptr && FocusedProcess->GetType() == STL::PROT::WINDOWED)
        {                       
            ProcessHandler::BufferSwapRequest = true;
        }

        if (NewFocus->GetType() == STL::PROT::WINDOWED)
        {                       
            ProcessHandler::BufferSwapRequest = true;
        }

        FocusedProcess = NewFocus;       

        if (NewFocus->GetType() != STL::PROT::FULLSCREEN)
        {
            FocusedProcess->SetDepth(Processes.Length() - 1);
        }  
    }

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
        if (MovingWindow != nullptr)
        {                
            MovingWindow->SetPos(Mouse::Position + MovingWindowPosDelta);

            if (!Mouse::LeftHeld)
            {       
                FocusedProcess = MovingWindow;
                MovingWindow = 0;
            }       
            RedrawRequest = true;
        }
        else
        {
            for (int i = Processes.Length(); i --> 0; )
            {                    
                if (Mouse::LeftHeld && Processes[i]->GetType() == STL::PROT::WINDOWED)
                {
                    if (STL::Contains(Processes[i]->GetPos() - FRAME_OFFSET, Processes[i]->GetPos() + STL::Point(Processes[i]->GetSize().X, 0), Mouse::Position)) // If over topbar
                    {                        
                        STL::Point CloseButtonPos = Processes[i]->GetCloseButtonPos();
                        if (STL::Contains(CloseButtonPos, CloseButtonPos + CLOSE_BUTTON_SIZE, Mouse::Position)) //If over close button
                        {
                            KillProcess(Processes[i]->GetID());
                            break;
                        }

                        MovingWindow = Processes[i];
                        MovingWindowPosDelta = Processes[i]->GetPos() - Mouse::Position;

                        SetFocusedProcess(Processes[i]);    
                        break;
                    }
                }  

                if (Processes[i]->Contains(Mouse::Position)) //If over window
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
                RedrawRequest = true;
                return true;
            }
        }

        return false;
    }

    uint64_t StartProcess(STL::PROC Procedure)
    {                    
        for (int i = 0; i < Processes.Length(); i++)
        {
            if (Processes[i]->GetProcedure() == Procedure)
            {
                return 0;
            }
        }

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
                    if (i == Processes.Length())
                    {
                        BufferSwapRequest = true;
                    }
                    else
                    {
                        RedrawRequest = true;
                    }
                }
                break;
                case STL::PROR::DRAW:
                {
                    Processes[i]->Draw();
                    if (i == Processes.Length())
                    {
                        BufferSwapRequest = true;
                    }
                    else
                    {
                        RedrawRequest = true;
                    }
                }
                break;
                case STL::PROR::KILL:
                {
                    KillProcess(Processes[i]->GetID());
                    i--;
                    RedrawRequest = true;
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

            if (RedrawRequest)
            {        
                for (int i = 0; i < Processes.Length(); i++)
                {
                    Processes[i]->Render();
                }    
                Renderer::SwapBuffers();
                RedrawRequest = false;
                BufferSwapRequest = false;     
            }
            else if (BufferSwapRequest)
            {                
                Renderer::SwapBuffers();
                BufferSwapRequest = false;
            }

            if (Processes.Length() == 0)
            {
                StartProcess(tty::Procedure);
                FocusedProcess = Processes[0];
            }
            
            asm("HLT");
        }
    }
}