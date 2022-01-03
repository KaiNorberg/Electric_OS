#include "Renderer.h"
#include "../PIT/PIT.h"
#include "../String/cstr.h"

#include <stdint.h>

namespace Renderer
{
    Framebuffer* Screenbuffer;
    PSF_FONT* CurrentFont;

    Point CursorPos;

    void Init(Framebuffer* framebuffer, PSF_FONT* PSF_Font)
    {
        Screenbuffer = framebuffer;

        CurrentFont = PSF_Font;
        CursorPos.X = 0;
        CursorPos.Y = 0;
    }

    void PutPixel(Point Pixel, ARGB Color)
    {
        if (Pixel.X > Screenbuffer->Width || Pixel.X < 0 || Pixel.Y > Screenbuffer->Height || Pixel.Y < 0)
        {
            return;
        }

        *(ARGB*)((uint64_t)Screenbuffer->Base + Pixel.X * 4 + Pixel.Y * Screenbuffer->PixelsPerScanline * 4) = Color;
    }

    ARGB GetPixel(Point Pixel)
    {        
        if (Pixel.X > Screenbuffer->Width || Pixel.X < 0 || Pixel.Y > Screenbuffer->Height || Pixel.Y < 0)
        {
            return ARGB(0);
        }

        return *(ARGB*)((uint64_t)Screenbuffer->Base + Pixel.X * 4 + Pixel.Y * Screenbuffer->PixelsPerScanline * 4);
    }

    void PutChar(char chr, ARGB Background, ARGB Foreground, Point Pos, uint8_t Scale)
    {
        char* Glyph = CurrentFont->glyphBuffer + (chr * CurrentFont->PSF_header->charsize);

        for (uint64_t y = 0; y < 16 * Scale; y++)
        {
            for (uint64_t x = 0; x < 8 * Scale; x++)
            {
                if ((*Glyph & (0b10000000 >> x / Scale)) > 0)
                {
                    *(ARGB*)((uint64_t)Screenbuffer->Base + (x + Pos.X) * 4 + (y + Pos.Y) * Screenbuffer->PixelsPerScanline * 4) = Foreground;
                }
                else
                {
                    *(ARGB*)((uint64_t)Screenbuffer->Base + (x + Pos.X) * 4 + (y + Pos.Y) * Screenbuffer->PixelsPerScanline * 4) = Background;
                }
            }
            if (y % Scale == 0)
            {
                Glyph++;
            }
        }
    }

    void Print(const char* str, ARGB Background, ARGB Foreground, uint8_t Scale)
    {
        char* chr = (char*)str;

        while (*chr != 0)
        {
            Print(*chr, Background, Foreground, Scale);
            chr++;
        }
    }

    void Print(char Chr, ARGB Background, ARGB Foreground, uint8_t Scale)
    {
        if (Chr == '\n')
        {
            CursorPos.Y += 16 * Scale;
        }
        else if (Chr == '\r')
        {
            CursorPos.X = 0;
        }
        else
        {        
            if (CursorPos.X + 16 * Scale > Screenbuffer->Width)
            {
                CursorPos.X = 0;
                CursorPos.Y += 16 * Scale;
            }
            if (CursorPos.Y + 16 > Screenbuffer->Height)
            {
                ScrollUp(32);
                CursorPos.Y -= 32;
            }

            PutChar(Chr, Background, Foreground, CursorPos, Scale);
            CursorPos.X += 8 * Scale;
        }
    }

    void ScrollUp(uint64_t Amount)
    {
        uint64_t Offset = Screenbuffer->PixelsPerScanline * Amount;
        Memory::Copy(Screenbuffer->Base + Offset, Screenbuffer->Base, Screenbuffer->Size - Offset * 4);
        Memory::Set(Screenbuffer->Base + Screenbuffer->PixelsPerScanline * (Screenbuffer->Height - Amount), 0, Offset * 4);

       // PIT::Sleep(1);
    }

    void Clear(ARGB Color)
    {
        CursorPos.X = 0;
        CursorPos.Y = 0;

        Memory::Set(Screenbuffer->Base, 0, Screenbuffer->Size);
    }

    Point GetScreenSize()
    {
        return Point(Screenbuffer->Width, Screenbuffer->Height);
    }
}
