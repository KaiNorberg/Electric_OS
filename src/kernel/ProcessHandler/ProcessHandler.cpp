#include "ProcessHandler.h"
#include "Process.h"

#include "STL/Math/Point.h"
#include "STL/List/List.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/Memory/Memory.h"

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

    volatile bool MouseIntCalled = false;
    volatile bool KeyboardIntCalled = false;
    volatile bool PITIntCalled = false;

    void SendMessage(STL::PROM Message, STL::PROI Input)
    {
        for (int i = 0; i < Processes.Length(); i++)
        {
            Processes[i].SendMessage(Message, Input);
        }
    }

    void KeyBoardInterupt()
    {
        KeyboardIntCalled = true;
    }

    void MouseInterupt()
    {
        MouseIntCalled = true;
    }   

    void PITInterupt()
    {
        PITIntCalled = true;
    }

    void StartProcess(STL::PROC Procedure)
    {
        Processes.Append(Process(Procedure));

        STL::PINFO Info;
        Processes.Last().SendMessage(STL::PROM::INIT, &Info);

        Processes.Last().Depth = Info.Depth;
        Processes.Last().Type = Info.Type;

        Processes.Last().Pos = STL::Point(Info.Left, Info.Top);

        if (Info.Type == STL::PROT::FULLSCREEN)
        {        
            Processes.Last().Pos = STL::Point(0, 0);
            Processes.Last().FrameBuffer = *Renderer::Screenbuffer;
            Processes.Last().FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(Renderer::Screenbuffer->Size);
            Processes.Last().FrameBuffer.Clear();
            Processes.Last().Draw();
            FocusedProcess = Processes.Length() - 1;
        }
    }

    void Loop()
    {
        StartProcess(tty::Procedure);

        while (true)
        {                
            if (PITIntCalled)
            {
                for (int i = 0; i < Processes.Length(); i++)
                {
                    if (i == FocusedProcess || Processes[i].Type == STL::PROT::BACKGROUND)
                    {
                        uint64_t Tick = PIT::Ticks;
                        Processes[i].SendMessage(STL::PROM::TICK, &Tick);
                    }
                }
                PITIntCalled = false;
            }
            else if (KeyboardIntCalled)
            {
                for (int i = 0; i < Processes.Length(); i++)
                {
                    if (i == FocusedProcess || Processes[i].Type == STL::PROT::BACKGROUND)
                    {
                        uint8_t Key = KeyBoard::GetKeyPress();
                        Processes[i].SendMessage(STL::PROM::KEYPRESS, &Key);
                    }
                }
                KeyboardIntCalled = false;
            }
            else if (MouseIntCalled)
            {
                Renderer::RedrawMouse();
                MouseIntCalled = false;
            }

            asm("HLT");
        }
    }
}