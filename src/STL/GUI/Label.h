#pragma once

#include "STL/Math/Point.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/String/String.h"
#include "STL/Process/Process.h"

namespace STL
{
    enum class LabelAlign
    {
        Negative,
        Center,
        Positive
    };

    enum class LabelStyle
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

        uint64_t Scale;

        void Draw(Framebuffer* Buffer);

        Label() = default;

        Label(const char* Text, Point TopLeft, Point BottomRight);

    private:

    };
}