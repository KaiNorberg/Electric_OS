#include "tty.h"

#include "STL/Graphics/Graphics.h"
#include "STL/String/String.h"
#include "STL/String/cstr.h"
#include "STL/System/System.h"

namespace tty
{
    /*STL::Point StartCursorPos = Renderer::CursorPos;
    int i = 0;*/

    uint64_t PrevTick = 0;

    STL::String Text;
    char Command[64];
    STL::Point CursorPos = STL::Point(0, 16);

    bool RedrawText = false;
    bool DrawUnderline = false;
    bool ClearCommand = false;

    void Write(const char* cstr)
    {
        Text += cstr;
    }

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;
            Info->Type = STL::PROT::FULLSCREEN;
            
            Text = "";
            Command[0] = 0;
            CursorPos = STL::Point(0, 16);

            Write("Welcome to the tty of ");
            Write(OS_VERSION);
            Write("!\n\r");
            Write("Type \"help\" for a list of all available commands.\n\n\r");
            Write(STL::System("sysfetch"));
            Write("\n\r");
            Write("> ");

            RedrawText = true;
        }
        break;
        case STL::PROM::TICK:
        {   
            uint64_t CurrentTick = *(uint64_t*)Input;

            if (PrevTick + 50 < CurrentTick)
            {
                DrawUnderline = !DrawUnderline;
                PrevTick = CurrentTick;

                return STL::PROR::REDRAW;
            }
        }
        break;
        case STL::PROM::DRAW:
        {
            STL::Framebuffer* Buffer = (STL::Framebuffer*)Input;

            if (RedrawText)
            {
                //TODO: Implement scrolling

                STL::Print(Buffer, Text.cstr(), CursorPos, 1, STL::ARGB(255), STL::ARGB(0));
                Text = "";
                RedrawText = false;
            }

            if (ClearCommand)
            {
                for (int i = 0; i < STL::Length(Command) + 2; i++)
                {
                    STL::PutChar(Buffer, ' ', STL::Point(CursorPos.X + 8 * i, CursorPos.Y), 1, STL::ARGB(255), STL::ARGB(0));
                }
                ClearCommand = false;
            }
            
            STL::Point Temp = CursorPos;
            STL::Print(Buffer, Command, Temp, 1, STL::ARGB(255), STL::ARGB(0));

            if (DrawUnderline)
            {
                STL::PutChar(Buffer, '_', Temp, 1, STL::ARGB(255), STL::ARGB(0));
            }
            else
            {
                STL::PutChar(Buffer, ' ', Temp, 1, STL::ARGB(255), STL::ARGB(0));
            }
        }
        break;
        case STL::PROM::KEYPRESS:
        {
            uint8_t Key = *(uint8_t*)Input;

            if (Key == ENTER)
            {   
                Text += Command;
                Text += "\n\r";
                Text += STL::System(Command);
                Text += "\n\r> ";
                Command[0] = 0;
                RedrawText = true;
            }
            else if (Key == BACKSPACE)
            {
                uint64_t CommandLength = STL::Length(Command);

                if (CommandLength != 0)
                {
                    Command[CommandLength - 1] = 0;
                    Command[CommandLength] = 0;
                    ClearCommand = true;
                }
            }
            else
            {               
                uint64_t CommandLength = STL::Length(Command);
                Command[CommandLength] = Key;
                Command[CommandLength + 1] = 0;
            }

            return STL::PROR::REDRAW;
        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}