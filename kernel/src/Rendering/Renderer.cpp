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

    void PutChar(char chr, ARGB const& Color, Point const& Pos)
    {
        char* Glyph = CurrentFont->glyphBuffer + (chr * CurrentFont->psf1_header->charsize);

        for (uint64_t y = Pos.Y; y < Pos.Y + 16; y++)
        {
            for (uint64_t x = Pos.X; x < Pos.X + 8; x++)
            {
                if ((*Glyph & (0b10000000 >> (x - Pos.X))) > 0)
                {
                    Screenbuffer->SetPixel(Point(x, y), Color);
                }
            }
            Glyph++;
        }
    }

    void Print(const char* str, ARGB Color)
    {
        char* chr = (char*)str;

        while (*chr != 0)
        {
            Print(*chr, Color);
            chr++;
        }
    }

    void Print(char Chr, ARGB Color)
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
            if (CursorPos.X + 16 > Screenbuffer->Width)
            {
                CursorPos.X = 0;
                CursorPos.Y += 16;
            }

            PutChar(Chr, Color, CursorPos);
            CursorPos.X += 8;
        }
    }

    void Clear(ARGB Color)
    {
        CursorPos.X = 0;
        CursorPos.Y = 0;

        for (int Y = 0; Y < Screenbuffer->Height; Y++)
        {
            for (int X = 0; X < Screenbuffer->Width; X++)
            {
                Screenbuffer->SetPixel(Point(X, Y), Color);
            }
        }
    }

    Point GetScreenSize()
    {
        return Point(Screenbuffer->Width, Screenbuffer->Height);
    }
}
