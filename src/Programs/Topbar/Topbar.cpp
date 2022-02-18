#include "Topbar.h"

#include "STL/Graphics/Framebuffer.h"
#include "STL/System/System.h"
#include "STL/GUI/Button.h"
#include "STL/GUI/Label.h"

#include "STL/String/cstr.h"

#include "Programs/Desktop/Desktop.h"

#define TOPBAR_PADDING 8 + RAISEDWIDTH * 4 
#define BUTTONGAP 125
#define BUTTONWIDTH 100


namespace Topbar
{                    
    STL::Label TimeDateLabel;

    STL::ARGB BackgroundColor;

    STL::Button SystemButton;
    STL::Button StartButton;

    void(*CurrentAnimation)(STL::Framebuffer*);
    uint64_t AnimationCounter = 0;

    int64_t SystemMenuID = -1;
    int64_t StartMenuID = -1;

    inline void StartAnimation(void(*Animation)(STL::Framebuffer*))
    {
        AnimationCounter = 0;
        CurrentAnimation = Animation;
    }

    void OpenAnimation(STL::Framebuffer* Buffer)
    {
        if (AnimationCounter == 0)
        {
            Buffer->DrawRect(STL::Point(0, 0), STL::Point(Buffer->Width, Buffer->Height), Desktop::BackgroundColor);
        }
        else if (AnimationCounter > Buffer->Height - RAISEDWIDTH)
        {
            StartAnimation(nullptr);   
            return;
        }

        uint64_t Offset = ((Buffer->Height - RAISEDWIDTH) - AnimationCounter);

        Buffer->DrawRaisedRect(STL::Point(RAISEDWIDTH, RAISEDWIDTH), STL::Point(Buffer->Width - RAISEDWIDTH, AnimationCounter), BackgroundColor);

        TimeDateLabel.TopLeft.Y = 4 + RAISEDWIDTH * 2 - Offset;
        TimeDateLabel.BottomRight.Y = Buffer->Height - 4 - RAISEDWIDTH * 2 - Offset;

        SystemButton.TopLeft.Y = 4 + RAISEDWIDTH * 2 - Offset;
        SystemButton.BottomRight.Y = Buffer->Height - 4 - RAISEDWIDTH * 2 - Offset;

        StartButton.TopLeft.Y = 4 + RAISEDWIDTH * 2 - Offset;
        StartButton.BottomRight.Y = Buffer->Height - 4 - RAISEDWIDTH * 2 - Offset;
    }

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {        
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;
            Info->Type = STL::PROT::FRAMELESSWINDOW;
            Info->Depth = 1;
            Info->Left = 0;
            Info->Top = 0;
            Info->Width = 1920;
            Info->Height = 16 + TOPBAR_PADDING + RAISEDWIDTH;
            Info->Title = "Topbar";

            BackgroundColor = STL::ARGB(255, 200, 200, 200);

            SystemMenuID = -1;

            TimeDateLabel = STL::Label("TEMP", STL::Point(Info->Width / 2 - 100, 4 + RAISEDWIDTH * 2), STL::Point(Info->Width / 2 + 100, Info->Height - 4 - RAISEDWIDTH * 2));
            TimeDateLabel.Style = STL::LabelStyle::Sunken;

            SystemButton = STL::Button(BackgroundColor, "System", STL::Point(Info->Width - BUTTONGAP - BUTTONWIDTH / 2, 4 + RAISEDWIDTH * 2), STL::Point(Info->Width - BUTTONGAP + BUTTONWIDTH / 2, Info->Height - 4 - RAISEDWIDTH * 2));
            StartButton = STL::Button(BackgroundColor, "Start", STL::Point(BUTTONGAP - BUTTONWIDTH / 2, 4 + RAISEDWIDTH * 2), STL::Point(BUTTONGAP + BUTTONWIDTH / 2, Info->Height - 4 - RAISEDWIDTH * 2));

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

            SystemButton.Draw(Buffer);
            StartButton.Draw(Buffer);
            TimeDateLabel.Draw(Buffer);
        }
        break;
        case STL::PROM::TICK:
        {
            uint64_t CurrentTick = *(uint64_t*)Input;

            if (CurrentAnimation != nullptr || CurrentTick % 100 == 0)
            {
                TimeDateLabel.Text = STL::System("time");
                TimeDateLabel.Text += " ";
                TimeDateLabel.Text += STL::System("date");

                return STL::PROR::DRAW;
            }
        }
        break;
        case STL::PROM::MOUSE:
        {
            STL::MINFO MouseInfo = *(STL::MINFO*)Input;

            if (SystemButton.IsToggled(MouseInfo))
            {
                if (SystemMenuID == -1)
                {
                    SystemMenuID = STL::ToInt(STL::System("start systemmenu"));
                }
                else
                {
                    STL::String Command = "kill ";
                    Command += STL::ToString(SystemMenuID);
                    STL::System(Command.cstr());
                    SystemMenuID = -1;
                }
            }
            
            if (StartButton.IsToggled(MouseInfo))
            {
                if (StartMenuID == -1)
                {
                    StartMenuID = STL::ToInt(STL::System("start startmenu"));
                }
                else
                {
                    STL::String Command = "kill ";
                    Command += STL::ToString(StartMenuID);
                    STL::System(Command.cstr());
                    StartMenuID = -1;
                }
            }

            return STL::PROR::DRAW;
        }
        break;
        default:
        {

        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}