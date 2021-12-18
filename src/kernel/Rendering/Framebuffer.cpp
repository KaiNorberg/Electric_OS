#include "Framebuffer.h"

void Framebuffer::SetPixel(uint64_t X, uint64_t Y, ARGB Color)
{
    *(uint64_t*)((uint64_t)this->Base + X * 4 + Y * this->PixelsPerScanline * 4) = Color.ToInt();
}
