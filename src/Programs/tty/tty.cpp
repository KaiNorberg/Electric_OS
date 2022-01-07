#include "tty.h"

#include "STL/Graphics/Framebuffer.h"

namespace tty
{
    /*STL::Point StartCursorPos = Renderer::CursorPos;
    uint64_t PrevTick = PIT::Ticks;
    bool Alternate = false;
    int i = 0;*/

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;
            Info->Type = STL::PROT::FULLSCREEN;

            /*Print("\n\r");
            Print("Welcome to the tty of ");
            Print(OS_VERSION);
            Print("!\n\r");
            Print("Type \"help\" for a list of all available commands.\n\n\r");
            RunCommand("sysfetch");
            Print("\n\r");

            Print("> ");*/
        }
        break;
        case STL::PROM::TICK:
        {
            /*if (PrevTick + PIT::GetFrequency() / 2 < PIT::Ticks)
            {
                Alternate = !Alternate;
                PrevTick = PIT::Ticks;

                if (Alternate)
                {
                    Renderer::PutChar('_', Background, Foreground, Renderer::CursorPos, Scale);
                }
                else
                {
                    Renderer::PutChar(' ', Background, Foreground, Renderer::CursorPos, Scale);
                }
            }*/
        }
        break;
        case STL::PROM::DRAW:
        {
            STL::Framebuffer* Buffer = (STL::Framebuffer*)Input;
         

        }
        break;
        case STL::PROM::KEYPRESS:
        {
           /* uint8_t Key = *(uint8_t*)Input;

            if (Key == ENTER || STL::Length(Input) >= sizeof(Input)/sizeof(Input[0]) - 1)
            {   
                
            }
            else if (Key == BACKSPACE && STL::Length(Input) != 0)
            {
                uint64_t Length = STL::Length(Input);
                Input[Length - 1] = 0;
                Input[Length] = 0;
                Input[Length + 1] = 0;
            }
            else
            {               
                uint64_t Length = STL::Length(Input);
                Input[Length] = Key;
                Input[Length + 1] = 0;
            }*/

            return STL::PROR::REDRAW;
        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}