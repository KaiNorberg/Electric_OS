#pragma once

#include "../Rendering/Framebuffer.h"
#include "../Math/Point.h"

namespace Draw
{   
    void LineX(Framebuffer* Buffer, int x1, int x2, int y, RGBA Color);

    void LineY(Framebuffer* Buffer, int x, int y1, int y2, RGBA Color);

    void Pixel(Framebuffer* Buffer, Point Pos, RGBA Color);

    void Circle(Framebuffer* Buffer, Point Pos, RGBA Color, int64_t Radius);

    void ThinCircle(Framebuffer* Buffer, Point Pos, RGBA Color, int64_t Radius, int64_t Thickness);

    void Rectangle(Framebuffer* Buffer, Point LeftTop, Point RightBottom, RGBA Color);

    void ThinRectangle(Framebuffer* Buffer, Point LeftTop, Point RightBottom, RGBA Color);
}