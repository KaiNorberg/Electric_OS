#include "Renderer.h"

#include <stdint.h>

extern uint64_t _BACKBUFFER;

namespace Renderer
{
    Framebuffer Backbuffer;
    Framebuffer* Screenbuffer;
    PSF_FONT* CurrentFont;

    Point CursorPos;

    void Init(Framebuffer* framebuffer, PSF_FONT* PSF_Font)
    {
        Screenbuffer = framebuffer;
        Backbuffer = *Screenbuffer;
        Backbuffer.Base = (ARGB*)&_BACKBUFFER;

        CurrentFont = PSF_Font;
        CursorPos.X = 0;
        CursorPos.Y = 0;
    }

    void PutPixel(Point Pixel, ARGB Color)
    {
        *(uint64_t*)((uint64_t)Backbuffer.Base + Pixel.X * 4 + Pixel.Y * Backbuffer.PixelsPerScanline * 4) = Color.ToInt();
    }

    void PutChar(char chr, ARGB Color, Point Pos)
    {
        char* Glyph = CurrentFont->glyphBuffer + (chr * CurrentFont->PSF_header->charsize);

        for (uint64_t y = Pos.Y; y < Pos.Y + 16; y++)
        {
            for (uint64_t x = Pos.X; x < Pos.X + 8; x++)
            {
                if ((*Glyph & (0b10000000 >> (x - Pos.X))) > 0)
                {
                    PutPixel(Point(x, y), Color);
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
            if (CursorPos.X + 16 > Backbuffer.Width)
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

        for (int Y = 0; Y < Backbuffer.Height; Y++)
        {
            for (int X = 0; X < Backbuffer.Width; X++)
            {
                PutPixel(Point(X, Y), Color);
            }
        }
    }

    void SwapBuffers()
    {
        Memory::Copy(Backbuffer.Base, Screenbuffer->Base, Backbuffer.Size);
    }

    Point GetScreenSize()
    {
        return Point(Backbuffer.Width, Backbuffer.Height);
    }
}
