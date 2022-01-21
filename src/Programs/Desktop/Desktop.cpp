#include "Desktop.h"

#include "STL/System/System.h"
#include "STL/Graphics/Framebuffer.h"

namespace Desktop
{
    uint64_t Frame = 0;

    bool StartAnimationPlayed = false;

    STL::ARGB BackgroundColor = STL::ARGB(255, 59, 110, 165);

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;
            Info->Type = STL::PROT::FULLSCREEN;

            Frame = 0;
            StartAnimationPlayed = false;
            BackgroundColor = STL::ARGB(255, 0, 0, 255);
        }
        break;
        case STL::PROM::DRAW:
        {
            STL::Framebuffer* Buffer = (STL::Framebuffer*)Input;
        
            Frame++;

            if (!StartAnimationPlayed)
            {            
                uint64_t Step = (Buffer->Width / 75);
                uint64_t Width = Frame * Step;

                if (Width > Buffer->Width)
                {
                    Buffer->DrawRect(STL::Point(0, 0), STL::Point(Buffer->Width, Buffer->Height), BackgroundColor);
                    StartAnimationPlayed = true;

                    STL::System("set drawmouse 1");
                    STL::System("start topbar");
                }
                else
                {
                    Buffer->DrawRect(STL::Point(Buffer->Width / 2 + Width / 2 - Step, 0), STL::Point(Buffer->Width / 2 + Width / 2, Buffer->Height), BackgroundColor);
                    Buffer->DrawRect(STL::Point(Buffer->Width / 2 - Width / 2 - Step, 0), STL::Point(Buffer->Width / 2 - Width / 2, Buffer->Height), BackgroundColor);
                }
            }        
        }
        break;
        case STL::PROM::TICK:
        {
            if (!StartAnimationPlayed)
            {
                return STL::PROR::DRAW;
            }
        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}