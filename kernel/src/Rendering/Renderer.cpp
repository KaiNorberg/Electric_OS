#include "Renderer.h"

#include <stdint.h>

namespace Renderer
{
    Framebuffer* Screenbuffer;
    PSF1_FONT* CurrentFont;

    Point CursorPos;

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
            Print(*chr, Color);
            chr++;
        }
    }

    void Print(char Chr, uint32_t Color)
    {
        if (Chr == '\n')
        {
            CursorPos.Y += 16;
        }
        else if (Chr == '\r')
        {
            CursorPos.X = 0;
        }
        else
        {
            PutChar(Chr, Color, CursorPos);
            CursorPos.X += 8;
        }
    }

    void Clear(uint32_t Color)
    {
        CursorPos.X = 0;
        CursorPos.Y = 0;

        for (int Y = 0; Y < Screenbuffer->Height; Y++)
        {
            uint64_t pixPtrBase = (uint64_t)Screenbuffer->Base + (Screenbuffer->PixelsPerScanline * 4 * Y);
            for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + Screenbuffer->PixelsPerScanline * 4); pixPtr ++)
            {
                *pixPtr = Color;
            }
        }
    }
}
