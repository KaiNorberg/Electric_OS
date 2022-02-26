#include "Label.h"

#include "STL/String/cstr.h"

namespace STL
{
    void Label::Draw(Framebuffer* Buffer)
    {
        switch (this->Style)
        {
        case LabelStyle::Flat:
        {
            Buffer->DrawRect(this->TopLeft, this->BottomRight, this->Background);
        }
        break;
        case LabelStyle::Sunken:
        {
            Buffer->DrawSunkenRect(this->TopLeft, this->BottomRight, this->Background);           
        }
        break;        
        case LabelStyle::Raised:
        {
            Buffer->DrawRaisedRect(this->TopLeft, this->BottomRight, this->Background);            
        }
        break;
        }

        STL::Point TextPos;

        //Causes a crash on some systems, for a probably really good reason.
        /*switch (this->VerticalAlign)
        {
        case LabelAlign::Negative:
        {
            TextPos.Y = this->TopLeft.Y + RAISED_WIDTH * Scale;
        }
        break;
        case LabelAlign::Center:
        {
            TextPos.Y = (this->TopLeft.Y + this->BottomRight.Y) / 2 - (16 * Scale) / 2;         
        }
        break;        
        case LabelAlign::Positive:
        {
            TextPos.Y = this->BottomRight.Y - RAISED_WIDTH * Scale;
        }
        break;
        }*/
        
        TextPos.Y = (this->TopLeft.Y + this->BottomRight.Y) / 2 - (16 * Scale) / 2;         

        switch (this->HorizontalAlign)
        {
        case LabelAlign::Negative:
        {
            TextPos.X = this->TopLeft.X + RAISED_WIDTH * Scale;
        }
        break;
        case LabelAlign::Center:
        {
            TextPos.X = (this->TopLeft.X + this->BottomRight.X) / 2 - (STL::Length(this->Text.cstr()) * 8 * Scale) / 2;         
        }
        break;        
        case LabelAlign::Positive:
        {
            TextPos.X = this->BottomRight.X - RAISED_WIDTH * Scale - STL::Length(this->Text.cstr()) * 8 * Scale;
        }
        break;
        }

        Buffer->Print(this->Text.cstr(), TextPos, this->Scale, this->Foreground, this->Background);
    }

    Label::Label(const char* Text, Point TopLeft, Point BottomRight)
    {
        this->Text = Text;
        this->TopLeft = TopLeft;
        this->BottomRight = BottomRight;

        this->HorizontalAlign = LabelAlign::Center;
        this->VerticalAlign = LabelAlign::Center;
        this->Background = STL::ARGB(200);
        this->Foreground = STL::ARGB(60);
        this->Style = LabelStyle::Flat;
        this->Scale = 1;
    }
}