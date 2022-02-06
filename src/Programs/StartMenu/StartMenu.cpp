#include "StartMenu.h"

#include "STL/System/System.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/String/cstr.h"
#include "STL/GUI/Button.h"

namespace StartMenu
{        
    struct StartableProcess
    {
        STL::Button Button;
        STL::String Name;

        StartableProcess() = default;
    };

    const uint64_t StartableProcessesAmount = 2;
    StartableProcess StartableProcesses[StartableProcessesAmount];

    void(*CurrentAnimation)(STL::Framebuffer*);
    uint64_t AnimationCounter = 0;

    inline void StartAnimation(void(*Animation)(STL::Framebuffer*))
    {
        AnimationCounter = 0;
        CurrentAnimation = Animation;
    }

    void OpenAnimation(STL::Framebuffer* Buffer)
    {
        Buffer->DrawRaisedRect(STL::Point(RAISEDWIDTH, RAISEDWIDTH), STL::Point(Buffer->Width - RAISEDWIDTH, Buffer->Height - RAISEDWIDTH), STL::ARGB(200));
    
        StartAnimation(nullptr);
    }

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {    
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;

            StartableProcesses[0].Name = "Calculator";
            StartableProcesses[1].Name = "Terminal";

            for (uint64_t i = 0; i < StartableProcessesAmount; i++)
            {
                STL::Point TopLeft = STL::Point(RAISEDWIDTH * 3, RAISEDWIDTH * ((i + 1) * 3) + i * (RAISEDWIDTH * 2 + 25));
                STL::Point BottomRight = STL::Point(200 - RAISEDWIDTH * 3, RAISEDWIDTH * ((i + 1) * 3) + ((i + 1)) * (RAISEDWIDTH * 2 + 25));

                StartableProcesses[i].Button = STL::Button(STL::ARGB(200), StartableProcesses[i].Name.cstr(), TopLeft, BottomRight);
            }

            Info->Type = STL::PROT::FRAMELESSWINDOW;
            Info->Depth = 1;
            Info->Left = 25;
            Info->Top = 50;
            Info->Width = 200;
            Info->Height = StartableProcesses[StartableProcessesAmount - 1].Button.BottomRight.Y + RAISEDWIDTH * 3;
            Info->Title = "StartMenu";

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

            for (uint64_t i = 0; i < StartableProcessesAmount; i++)
            {
                StartableProcesses[i].Button.Draw(Buffer);
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
        case STL::PROM::MOUSE:
        {
            STL::MINFO MouseInfo = *(STL::MINFO*)Input;

            for (uint64_t i = 0; i < StartableProcessesAmount; i++)
            {
                if (StartableProcesses[i].Button.IsPressed(MouseInfo))
                {
                    STL::String Command;
                    Command = "start ";
                    Command += StartableProcesses[i].Name;
                    STL::System(Command.cstr());
                }
            }

            return STL::PROR::DRAW;
        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}