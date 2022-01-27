#include "Topbar.h"

#include "STL/Graphics/Framebuffer.h"
#include "STL/System/System.h"

#include "STL/String/cstr.h"

#include "Programs/Desktop/Desktop.h"

#define TOPBAR_PADDING 8 + RAISEDWIDTH * 4 

namespace Topbar
{                    
    STL::Point TimePos;
    STL::Point DatePos;

    STL::ARGB BackgroundColor;

    void(*CurrentAnimation)(STL::Framebuffer*);
    uint64_t AnimationCounter = 0;

    inline void StartAnimation(void(*Animation)(STL::Framebuffer*))
    {
        AnimationCounter = 0;
        CurrentAnimation = Animation;
    }

    void TextAnimation(STL::Framebuffer* Buffer)
    {            
        if (AnimationCounter / 5 < 10)
        {
            char* Time = (char*)STL::System("time");

            Time[AnimationCounter / 5] = 0;
            STL::Point Temp = TimePos;
            Buffer->Print(Time, Temp, 1, STL::ARGB(255, 60, 60, 60), BackgroundColor);
        }
        else if (AnimationCounter / 5 - 10 < 10)
        {
            char* Date = (char*)STL::System("date");

            Date[AnimationCounter / 5 - 10] = 0;
            STL::Point Temp = DatePos;
            Buffer->Print(Date, Temp, 1, STL::ARGB(255, 60, 60, 60), BackgroundColor);
        }
        else
        {
            StartAnimation(nullptr);   
        }
    }

    void OpenAnimation(STL::Framebuffer* Buffer)
    {
        if (AnimationCounter == 0)
        {
            Buffer->DrawRect(STL::Point(0, 0), STL::Point(Buffer->Width, Buffer->Height), Desktop::BackgroundColor);
        }
        else if (AnimationCounter > Buffer->Height - RAISEDWIDTH)
        {
            StartAnimation(TextAnimation);
        }
        else
        {            
            Buffer->DrawRaisedRect(STL::Point(RAISEDWIDTH, RAISEDWIDTH), STL::Point(Buffer->Width - RAISEDWIDTH, AnimationCounter), BackgroundColor);
            Buffer->DrawSunkenRect(
            STL::Point(Buffer->Width / 2 - 100, 4 + RAISEDWIDTH * 2 - ((Buffer->Height - RAISEDWIDTH) - AnimationCounter)), 
            STL::Point(Buffer->Width / 2 + 100, Buffer->Height - 4 - RAISEDWIDTH * 2 - ((Buffer->Height - RAISEDWIDTH) - AnimationCounter)), BackgroundColor);
        }    
    }

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {        
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;
            Info->Type = STL::PROT::FRAMELESSWINDOW;
            Info->Left = 0;
            Info->Top = 0;
            Info->Width = 1920;
            Info->Height = 16 + TOPBAR_PADDING + RAISEDWIDTH;

            TimePos = STL::Point(1920 / 2 - 9 * 8, TOPBAR_PADDING / 2);
            DatePos = STL::Point(1920 / 2 + 8, TOPBAR_PADDING / 2);

            BackgroundColor = STL::ARGB(255, 200, 200, 200);

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
            else
            {
                Buffer->DrawSunkenRect(STL::Point(Buffer->Width / 2 - 100, 4 + RAISEDWIDTH * 2), STL::Point(Buffer->Width / 2 + 100, Buffer->Height - 4 - RAISEDWIDTH * 2), BackgroundColor);

                char* Time = (char*)STL::System("time");
                STL::Point Temp = TimePos;
                Buffer->Print(Time, Temp, 1, STL::ARGB(255, 60, 60, 60), BackgroundColor);

                char* Date = (char*)STL::System("date");
                Temp = DatePos;
                Buffer->Print(Date, Temp, 1, STL::ARGB(255, 60, 60, 60), BackgroundColor);
            }
        }
        break;
        case STL::PROM::TICK:
        {
            uint64_t CurrentTick = *(uint64_t*)Input;

            if (CurrentAnimation != nullptr || CurrentTick % 100 == 0)
            {
                return STL::PROR::DRAW;
            }
        }
        case STL::PROM::MOUSE:
        {
            STL::MINFO MouseInfo = *(STL::MINFO*)Input;



            return STL::PROR::DRAW;
        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}