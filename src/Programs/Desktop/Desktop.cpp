#include "Desktop.h"

#include "STL/System/System.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/String/cstr.h"

#include "Programs/Topbar/Topbar.h"

namespace Desktop
{
    void(*CurrentAnimation)(STL::Framebuffer*);
    uint64_t AnimationCounter = 0;

    inline void StartAnimation(void(*Animation)(STL::Framebuffer*))
    {
        AnimationCounter = 0;
        CurrentAnimation = Animation;
    }

    STL::ARGB BackgroundColor = STL::ARGB(255, 59, 110, 165);

    void OpenAnimation(STL::Framebuffer* Buffer)
    {
        uint64_t Step = (Buffer->Width / 75);
        uint64_t Width = AnimationCounter * Step;

        if (Width > Buffer->Width)
        {
            STL::System("set drawmouse 1");
            STL::System("start topbar");
            StartAnimation(nullptr);

            Buffer->DrawRect(STL::Point(0, 0), STL::Point(Buffer->Width, Buffer->Height), BackgroundColor);
        }
        else
        {
            Buffer->DrawRect(STL::Point(Buffer->Width / 2 + Width / 2 - Step, 0), STL::Point(Buffer->Width / 2 + Width / 2, Buffer->Height), BackgroundColor);
            Buffer->DrawRect(STL::Point(Buffer->Width / 2 - Width / 2 - Step, 0), STL::Point(Buffer->Width / 2 - Width / 2, Buffer->Height), BackgroundColor);
        }
    }

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;
            Info->Type = STL::PROT::FULLSCREEN;

            BackgroundColor = STL::ARGB(255, 59, 110, 165);

            StartAnimation(OpenAnimation);
        }
        break;
        case STL::PROM::DRAW:
        {
            STL::Framebuffer* Buffer = (STL::Framebuffer*)Input;
            if (CurrentAnimation != nullptr)
            {
                CurrentAnimation(Buffer);
                AnimationCounter++;

                if (CurrentAnimation == nullptr)
                {
                    return STL::PROR::DRAW;
                }
            }   
        }
        break;
        case STL::PROM::TICK:
        {            
            if (CurrentAnimation != nullptr)
            {
                return STL::PROR::DRAW;
            }
        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}