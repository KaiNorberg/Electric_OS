#include "tty.h"

#include "../Rendering/Renderer.h"
#include "../UserInput/KeyBoard.h"

namespace tty
{
    uint8_t Scale; 

    ARGB Background;
    ARGB Foreground;

    char Input[128];

    void Print(const char* Message)
    {
        Renderer::Print(Message, Background, Foreground, Scale);
    }

    void tty()
    {
        Scale = 1;
        Background = ARGB(0);
        Foreground = ARGB(255);

        Renderer::Clear(Background);

        for (int i = 0; i < sizeof(Input)/sizeof(Input[0]); i++)
        {
            Input[i] = 0;
        }

        Print("Terminal\n\r");

        while (true)
        {
            Print("> ");

            Point StartCursorPos = Renderer::CursorPos;
            uint64_t PrevTick = PIT::Ticks;
            int i = 0;
            while(true)
            {
                uint8_t Key = KeyBoard::GetKeyPress();
                if (Key != 0)
                {
                    if (Key == ENTER || i >= sizeof(Input)/sizeof(Input[0]) - 1)
                    {
                        break;
                    }
                    else if (Key == BACKSPACE && i != 0)
                    {
                        Input[i] = 0;
                        Input[i + 1] = 0;
                        Renderer::CursorPos.X -= 8 * Scale;
                        Print(" ");
                        Renderer::CursorPos.X -= 8 * Scale;
                        i--;
                    }
                    else
                    {
                        Input[i] = Key;
                        Input[i + 1] = 0;
                        Renderer::CursorPos = StartCursorPos;
                        for (int j = 0; j <= i; j++)
                        {
                            Print(" ");
                        }
                        Renderer::CursorPos = StartCursorPos;
                        Print(Input);
                        i++;
                    }
                }
                else if (PrevTick + PIT::GetFrequency() < PIT::Ticks)
		        {
                    
                }
                asm("HLT");
            }
            //Handle input
            Print("\n\r");
            Print(Input);
            Input[0] = 0;
            Print("\n\r");
        }   
    }
}