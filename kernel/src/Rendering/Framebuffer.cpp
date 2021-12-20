#include "Framebuffer.h"

void Framebuffer::SetPixel(Point Pixel, ARGB Color)
{
    *(uint64_t*)((uint64_t)this->Base + Pixel.X * 4 + Pixel.Y * this->PixelsPerScanline * 4) = Color.ToInt();
}
