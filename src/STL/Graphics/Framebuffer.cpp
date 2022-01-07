#include "Framebuffer.h"

#include "STL/Memory/Memory.h"

namespace STL
{		
    void Framebuffer::Clear()
    {
        STL::SetMemory(this->Base, 0, this->Size);
    }

    ARGB Framebuffer::GetPixel(Point Pixel)
    {
        if (Pixel.X > this->Width || Pixel.X < 0 || Pixel.Y > this->Height || Pixel.Y < 0)
        {
            return ARGB(0);
        }

        return *(ARGB*)((uint64_t)this->Base + Pixel.X * 4 + Pixel.Y * this->PixelsPerScanline * 4);
    }

    void Framebuffer::PutPixel(Point Pixel, ARGB Color)
    {
        if (Pixel.X > this->Width || Pixel.X < 0 || Pixel.Y > this->Height || Pixel.Y < 0)
        {
            return;
        }

        *(ARGB*)((uint64_t)this->Base + Pixel.X * 4 + Pixel.Y * this->PixelsPerScanline * 4) = Color;
    }
}