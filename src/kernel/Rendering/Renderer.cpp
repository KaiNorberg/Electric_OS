#include "Renderer.h"

#include <stdint.h>

namespace Renderer
{
    Framebuffer* Screenbuffer;
    PSF1_FONT* CurrentFont;

    uint64_t XPos;
    uint64_t YPos;

    void Init(Framebuffer* framebuffer, PSF1_FONT* PSF1_Font)
    {
        Screenbuffer = framebuffer;

        CurrentFont = PSF1_Font;
        XPos = 0;
        YPos = 0;
    }

    void PutChar(char chr, ARGB const& Color, uint64_t X, uint64_t Y)
    {
        char* Glyph = CurrentFont->glyphBuffer + (chr * CurrentFont->psf1_header->charsize);

        for (uint64_t y = Y; y < Y + 16; y++)
        {
            for (uint64_t x = X; x < X + 8; x++)
            {
                if ((*Glyph & (0b10000000 >> (x - X))) > 0)
                {
                    Screenbuffer->SetPixel(x, y, Color);
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
            YPos += 16;
        }
        else if (Chr == '\r')
        {
            XPos = 0;
        }
        else
        {        
            if (XPos + 16 > Screenbuffer->Width)
            {
                XPos = 0;
                YPos += 16;
            }

            PutChar(Chr, Color, XPos, YPos);
            XPos += 8;
        }
    }

    void Clear(ARGB Color)
    {
        XPos = 0;
        YPos = 0;

        for (int Y = 0; Y < Screenbuffer->Height; Y++)
        {
            for (int X = 0; X < Screenbuffer->Width; X++)
            {
                Screenbuffer->SetPixel(X, Y, Color);
            }
        }
    }

    void GetScreenSize(uint64_t* X, uint64_t* Y)
    {
        if (X != nullptr)
        {
            *X = Screenbuffer->Width;
        }        
        if (Y != nullptr)
        {
            *Y = Screenbuffer->Height;
        }
    }
}
