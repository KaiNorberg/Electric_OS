#include "Renderer.h"

#include <stdint.h>

namespace Renderer
{
    Point CursorPos;

    Framebuffer* Screenbuffer;
    PSF1_FONT* CurrentFont;

    void Init(Framebuffer* framebuffer, PSF1_FONT* PSF1_Font)
    {
        Screenbuffer = framebuffer;
        CurrentFont = PSF1_Font;
        CursorPos.X = 0;
        CursorPos.Y = 0;
    }

    void PutChar(char chr, uint32_t Color, Point Pos)
    {
        char* FontPtr = CurrentFont->glyphBuffer + (chr * CurrentFont->psf1_header->charsize);

        if (Pos.X > Screenbuffer->Width)
        {
            Pos.X = 0;
            Pos.Y += 16;
        }

        for (uint64_t y = Pos.Y; y < Pos.Y + 16; y++)
        {
            for (uint64_t x = Pos.X; x < Pos.X + 8; x++)
            {
                if ((*FontPtr & (0b10000000 >> (x - Pos.X))) > 0)
                {
                    *(Screenbuffer->Base + x + (y * Screenbuffer->PixelsPerScanline)) = Color;
                }
            }
            FontPtr++;
        }
    }

    void Print(const char* str, uint32_t Color)
    {
        char* chr = (char*)str;

        while (*chr != 0)
        {
            if (*chr == '\n')
            {
                CursorPos.Y += 16;
            }
            else if (*chr == '\r')
            {
                CursorPos.X = 0;
            }
            else
            {
                PutChar(*chr, Color, CursorPos);
                CursorPos.X += 8;
            }
            chr++;
        }
    }
}
