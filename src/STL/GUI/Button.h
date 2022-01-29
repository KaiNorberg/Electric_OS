#pragma once

#include "STL/Math/Point.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/String/String.h"
#include "STL/Process/Process.h"

namespace STL
{
    class Button
    {
    public:

        ARGB Color;
        
        String Text;

        Point TopLeft;

        Point BottomRight;

        void Draw(Framebuffer* Buffer);

        bool IsToggled(MINFO MouseInfo);

        bool IsPressed(MINFO MouseInfo);

        bool IsDown();

        Button() = default;

        Button(ARGB Color, const char* Text, Point TopLeft, Point BottomRight);

    private:

        bool Pressed = false;
    };
}