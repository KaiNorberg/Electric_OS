#include "ProcessHandler.h"

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
    struct Process
    {
        STL::PROT Type;
        STL::PROC Procedure;
        STL::Framebuffer FrameBuffer;

        uint64_t Depth;

        STL::Point Pos;

        void Draw()
        {
            if (Type == STL::PROT::BACKGROUND)
            {
                return;
            }

            this->SendMessage(STL::PROM::DRAW, &FrameBuffer);

            if (this->Type == STL::PROT::WINDOWED)
            {
                /*for (int y = 0; y < FrameBuffer.Height; y++)
                {
                    if (y + Pos.Y > FrameBuffer.Height || X + Pos.X > FrameBuffer.Width)
                    {
                        return;
                    }

                    STL::CopyMemory(
                        FrameBuffer.Base + y * FrameBuffer.Width * 4,
                        Renderer::Screenbuffer->Base + (Pos.Y + y) * Renderer::Screenbuffer->Width * 4,
                        FrameBuffer.Width * 4
                    );
                }*/
            }
            else if (this->Type == STL::PROT::FULLSCREEN)
            {   
                Renderer::SwapBuffers();
            }
        }

        void SendMessage(STL::PROM Message, STL::PROI Input = nullptr)
        {
            STL::PROR Return = this->Procedure(Message, Input);

            if (Return == STL::PROR::REDRAW)
            {
                Draw();
            }
        }
    
        Process(STL::PROC Procedure)
        {
            this->Procedure = Procedure;
        } 
    };

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
            Processes.Last().FrameBuffer = Renderer::Backbuffer;
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
                Renderer::SwapBuffers();
                MouseIntCalled = false;
            }

            asm("HLT");
        }
    }
}