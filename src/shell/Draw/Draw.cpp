#include "Draw.h"

#include "../Math/Math.h"

namespace Draw
{
    void Pixel(Framebuffer* Buffer, Point Pos, RGBA Color)
    {
        if (Pos.X < 0 || Pos.X > Buffer->Width || Pos.Y < 0 || Pos.Y > Buffer->Height)
        {
            return;
        }

        *(uint32_t*)((uint64_t)Buffer->Base + Pos.X * 4 + Pos.Y * Buffer->PixelsPerScanline * 4) = Color.ToInt();
    }

    void Circle(Framebuffer* Buffer, Point Pos, RGBA Color, int64_t Radius)
    {
        for (int x = -Radius; x < Radius; x++)
        {
            int height = (int)Math::Sqrt(Radius * Radius - x * x);

            for (int y = -height; y < height; y++)
                Pixel(Buffer, Point(x + Pos.X, y + Pos.Y), Color);
        }
    }

    void Rectangle(Framebuffer* Buffer, Point LeftTop, Point RightBottom, RGBA Color)
    {
        for (int Y = LeftTop.Y; Y < RightBottom.Y; Y++)
        {
            for (int X = LeftTop.X; X < RightBottom.X; X++)
            {
                Draw::Pixel(Buffer, Point(X, Y), Color);
            }
        }
    }

    void ThinRectangle(Framebuffer* Buffer, Point LeftTop, Point RightBottom, RGBA Color)
    {
        for (uint64_t Y = LeftTop.Y; Y <= RightBottom.Y; Y++)
        {
            Draw::Pixel(Buffer, Point(LeftTop.X, Y), Color);
            Draw::Pixel(Buffer, Point(RightBottom.X, Y), Color);
        }

        for (uint64_t X = LeftTop.X; X <= RightBottom.X; X++)
        {
            Draw::Pixel(Buffer, Point(X, LeftTop.Y), Color);
            Draw::Pixel(Buffer, Point(X, RightBottom.Y), Color);
        }
    }
}