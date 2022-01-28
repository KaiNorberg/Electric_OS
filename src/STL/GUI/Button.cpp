#include "Button.h"

namespace STL
{
    void Button::Draw(Framebuffer* Buffer)
    {
        if (this->Pressed)
        {
            Buffer->DrawSunkenRect(this->TopLeft, this->BottomRight, this->Color);
        }
        else
        {
            Buffer->DrawRaisedRect(this->TopLeft, this->BottomRight, this->Color);
        }

        STL::Point TextPos = STL::Point((this->TopLeft.X + this->BottomRight.X) / 2 - (this->Text.Length() * 8) / 2, (this->TopLeft.Y + this->BottomRight.Y) / 2 - 8);
        Buffer->Print(this->Text.cstr(), TextPos, 1, STL::ARGB(60), this->Color);
    }

    bool Button::IsToggled(MINFO MouseInfo)
    {
        if (MouseInfo.LeftHeld && 
           (MouseInfo.Pos.X > this->TopLeft.X && MouseInfo.Pos.Y > this->TopLeft.Y && MouseInfo.Pos.X < this->BottomRight.X && MouseInfo.Pos.Y < this->BottomRight.Y))
        {
            this->Pressed = !this->Pressed;
        }
        return this->Pressed;
    } 

    bool Button::IsPressed(MINFO MouseInfo) 
    {
        this->Pressed = MouseInfo.LeftHeld && 
        (MouseInfo.Pos.X > this->TopLeft.X && MouseInfo.Pos.Y > this->TopLeft.Y && MouseInfo.Pos.X < this->BottomRight.X && MouseInfo.Pos.Y < this->BottomRight.Y);
        return this->Pressed;
    }

    Button::Button(ARGB Color, const char* Text, Point TopLeft, Point BottomRight)
    {
        this->Color = Color;
        this->Text = Text;
        this->TopLeft = TopLeft;
        this->BottomRight = BottomRight;
    }
}