#include "tty.h"

#include "STL/Graphics/Framebuffer.h"

namespace tty
{
    int Y = 0;

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;
            Info->Type = STL::PROT::FULLSCREEN;
        }
        break;
        case STL::PROM::DRAW:
        {
            STL::Framebuffer* Buffer = (STL::Framebuffer*)Input;
         
            if (Y > Buffer->Height - 16)
            {
                Y = 0;
                Buffer->Clear();
            }

            for (int i = 0; i < Buffer->Width; i++)
            {
                Buffer->PutPixel(STL::Point(i, Y), STL::ARGB(255, 255, 0, 0));
            }
        }
        break;
        case STL::PROM::KEYPRESS:
        {
            if (*((uint8_t*)Input) == 'a')
            {
                Y += 10;
            }
            return STL::PROR::REDRAW;
        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}