#include "Graphics.h"

namespace STL
{
    PSF_FONT* CurrentFont = nullptr;

    void SetFont(PSF_FONT* NewFont)
    {
        CurrentFont = NewFont;
    }

    void PutChar(Framebuffer* TargetBuffer, char chr, STL::Point Pos, uint8_t Scale)
    {
        char* Glyph = CurrentFont->glyphBuffer + (chr * CurrentFont->PSF_header->charsize);

        for (uint64_t y = 0; y < 16 * Scale; y++)
        {
            for (uint64_t x = 0; x < 8 * Scale; x++)
            {
                if ((*Glyph & (0b10000000 >> x / Scale)) > 0)
                {
                    *(STL::ARGB*)((uint64_t)TargetBuffer->Base + (x + Pos.X) * 4 + (y + Pos.Y) * TargetBuffer->PixelsPerScanline * 4) = Foreground;
                }
                else
                {
                    *(STL::ARGB*)((uint64_t)TargetBuffer->Base + (x + Pos.X) * 4 + (y + Pos.Y) * TargetBuffer->PixelsPerScanline * 4) = Background;
                }
            }
            if (y % Scale == 0)
            {
                Glyph++;
            }
        }
    }
}