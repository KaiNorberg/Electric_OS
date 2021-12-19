#pragma once

#include "../Rendering/Framebuffer.h"
#include "../Math/Point.h"

namespace Draw
{
    void Pixel(Framebuffer* Buffer, Point Pos, RGBA Color);

    void Circle(Framebuffer* Buffer, Point Pos, RGBA Color, int64_t Radius);

    void Rectangle(Framebuffer* Buffer, Point LeftTop, Point RightBottom, RGBA Color);

    void ThinRectangle(Framebuffer* Buffer, Point LeftTop, Point RightBottom, RGBA Color);
}