#include "Draw.h"

#include "../Math/Math.h"

namespace Draw
{
    void LineX(Framebuffer* Buffer, int x1, int x2, int y, RGBA Color)
    {
        while (x1 <= x2) Pixel(Buffer, Point(x1++, y), Color);
    }

    void LineY(Framebuffer* Buffer, int x, int y1, int y2, RGBA Color)
    {
        while (y1 <= y2) Pixel(Buffer, Point(x, y1++), Color);
    }

    void Pixel(Framebuffer* Buffer, Point Pos, RGBA Color)
    {
        Pos.X = Math::Clamp((uint32_t)Pos.X, (uint32_t)0, Buffer->Width);
        Pos.Y = Math::Clamp((uint32_t)Pos.Y, (uint32_t)0, Buffer->Height);

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

    void ThinCircle(Framebuffer* Buffer, Point Pos, RGBA Color, int64_t Radius, int64_t Thickness)
    {
        int inner = Radius - Thickness;
        int xo = Radius;
        int xi = inner;
        int y = 0;
        int erro = 1 - xo;
        int erri = 1 - xi;

        while(xo >= y) 
        {
            LineX(Buffer, Pos.X + xi, Pos.X + xo, Pos.Y + y,  Color);
            LineY(Buffer, Pos.X + y,  Pos.Y + xi, Pos.Y + xo, Color);
            LineX(Buffer, Pos.X - xo, Pos.X - xi, Pos.Y + y,  Color);
            LineY(Buffer, Pos.X - y,  Pos.Y + xi, Pos.Y + xo, Color);
            LineX(Buffer, Pos.X - xo, Pos.X - xi, Pos.Y - y,  Color);
            LineY(Buffer, Pos.X - y,  Pos.Y - xo, Pos.Y - xi, Color);
            LineX(Buffer, Pos.X + xi, Pos.X + xo, Pos.Y - y,  Color);
            LineY(Buffer, Pos.X + y,  Pos.Y - xo, Pos.Y - xi, Color);

            y++;

            if (erro < 0) 
            {
                erro += 2 * y + 1;
            } 
            else 
            {
                xo--;
                erro += 2 * (y - xo + 1);
            }

            if (y > inner) 
            {
                xi = y;
            } 
            else 
            {
                if (erri < 0) 
                {
                    erri += 2 * y + 1;
                } 
                else 
                {
                    xi--;
                    erri += 2 * (y - xi + 1);
                }
            }
        }
    }

    void Rectangle(Framebuffer* Buffer, Point LeftTop, Point RightBottom, RGBA Color)
    {
        for (int Y = LeftTop.Y; Y < RightBottom.Y; Y++)
        {
            for (int X = LeftTop.X; X < RightBottom.X; X++)
            {
                Pixel(Buffer, Point(X, Y), Color);
            }
        }
    }

    void ThinRectangle(Framebuffer* Buffer, Point LeftTop, Point RightBottom, RGBA Color)
    {
        for (uint64_t Y = LeftTop.Y; Y <= RightBottom.Y; Y++)
        {
            Pixel(Buffer, Point(LeftTop.X, Y), Color);
            Pixel(Buffer, Point(RightBottom.X, Y), Color);
        }

        for (uint64_t X = LeftTop.X; X <= RightBottom.X; X++)
        {
            Pixel(Buffer, Point(X, LeftTop.Y), Color);
            Pixel(Buffer, Point(X, RightBottom.Y), Color);
        }
    }
}