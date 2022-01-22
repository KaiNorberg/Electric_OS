#include "Topbar.h"

#include "STL/Graphics/Framebuffer.h"
#include "STL/System/System.h"

#include "STL/String/cstr.h"

#include "Programs/Desktop/Desktop.h"

#define TOPBAR_PADDING 10 

namespace Topbar
{                    
    STL::Point TimePos;
    STL::Point DatePos;

    STL::ARGB BackgroundColor;;

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
            Buffer->Print(Time, Temp, 1, STL::ARGB(212), STL::ARGB(0));
        }
        else if (AnimationCounter / 5 - 10 < 10)
        {
            char* Date = (char*)STL::System("date");

            Date[AnimationCounter / 5 - 10] = 0;
            STL::Point Temp = DatePos;
            Buffer->Print(Date, Temp, 1, STL::ARGB(212), STL::ARGB(0));
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
        else if (AnimationCounter - 1 > Buffer->Height - 2)
        {
            StartAnimation(TextAnimation);
        }
        else
        {
            for (int i = 0; i < Buffer->Width - 1; i++)
            {
                Buffer->PutPixel(STL::Point(i, AnimationCounter - 1), BackgroundColor);
            }
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
            Info->Width = 1980;
            Info->Height = 16 + TOPBAR_PADDING + 2;

            TimePos = STL::Point(1980 / 2 - 13 * 8, TOPBAR_PADDING / 2);
            DatePos = STL::Point(1980 / 2 - 3 * 8, TOPBAR_PADDING / 2);

            BackgroundColor = STL::ARGB(255, 0, 0, 0);

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
                Buffer->DrawRect(STL::Point(TimePos.X, 0), STL::Point(DatePos.X + 10 * 8, DatePos.Y + 16), STL::ARGB(0));

                char* Time = (char*)STL::System("time");
                STL::Point Temp = TimePos;
                Buffer->Print(Time, Temp, 1, STL::ARGB(212), STL::ARGB(0));

                char* Date = (char*)STL::System("date");
                Temp = DatePos;
                Buffer->Print(Date, Temp, 1, STL::ARGB(212), STL::ARGB(0));
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
        break;
        }

        return STL::PROR::SUCCESS;
    }
}