#pragma once

#include "STL/Math/Point.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/String/String.h"
#include "STL/Process/Process.h"

namespace STL
{
    enum class LabelAlign : uint32_t
    {
        Negative,
        Center,
        Positive
    };

    enum class LabelStyle : uint32_t
    {
        Flat,
        Sunken,
        Raised
    };

    class Label
    {
    public:

        ARGB Background;
                
        ARGB Foreground;

        String Text;

        Point TopLeft;

        Point BottomRight;

        LabelAlign HorizontalAlign;

        LabelAlign VerticalAlign;

        LabelStyle Style;

        uint8_t Scale;

        void Draw(Framebuffer* Buffer);

        Label() = default;

        Label(const char* Text, Point TopLeft, Point BottomRight);

    private:

    };
}