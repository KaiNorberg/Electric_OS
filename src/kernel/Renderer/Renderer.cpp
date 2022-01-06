#include "Renderer.h"
#include "../PIT/PIT.h"
#include "../String/cstr.h"
#include "../Memory/Heap.h"
#include "../UserInput/Mouse.h"

#include <stdint.h>

namespace Renderer
{
    Framebuffer* Frontbuffer;
    Framebuffer Backbuffer;

    PSF_FONT* CurrentFont;

    Point CursorPos;

    ARGB Background;
    ARGB Foreground;

    bool DrawMouse; 

    void Init(Framebuffer* framebuffer, PSF_FONT* PSF_Font)
    {
        Frontbuffer = framebuffer;

        Backbuffer = *Frontbuffer;
        Backbuffer.Base = (ARGB*)Heap::Allocate(Backbuffer.Size);

        CurrentFont = PSF_Font;

        CursorPos.X = 0;
        CursorPos.Y = 0;

        Background = ARGB(0);
        Foreground = ARGB(255);

        DrawMouse = false;
    }

    void PutPixel(Point Pixel, ARGB Color)
    {
        if (Pixel.X > Backbuffer.Width || Pixel.X < 0 || Pixel.Y > Backbuffer.Height || Pixel.Y < 0)
        {
            return;
        }

        *(ARGB*)((uint64_t)Backbuffer.Base + Pixel.X * 4 + Pixel.Y * Backbuffer.PixelsPerScanline * 4) = Color;
    }

    void PutPixelFront(Point Pixel, ARGB Color)
    {
        if (Pixel.X > Frontbuffer->Width || Pixel.X < 0 || Pixel.Y > Frontbuffer->Height || Pixel.Y < 0)
        {
            return;
        }

        *(ARGB*)((uint64_t)Frontbuffer->Base + Pixel.X * 4 + Pixel.Y * Frontbuffer->PixelsPerScanline * 4) = Color;
    }

    ARGB GetPixel(Point Pixel)
    {        
        if (Pixel.X > Backbuffer.Width || Pixel.X < 0 || Pixel.Y > Backbuffer.Height || Pixel.Y < 0)
        {
            return ARGB(0);
        }

        return *(ARGB*)((uint64_t)Backbuffer.Base + Pixel.X * 4 + Pixel.Y * Backbuffer.PixelsPerScanline * 4);
    }

    ARGB GetPixelFront(Point Pixel)
    {        
        if (Pixel.X > Frontbuffer->Width || Pixel.X < 0 || Pixel.Y > Frontbuffer->Height || Pixel.Y < 0)
        {
            return ARGB(0);
        }

        return *(ARGB*)((uint64_t)Frontbuffer->Base + Pixel.X * 4 + Pixel.Y * Frontbuffer->PixelsPerScanline * 4);
    }

    void PutChar(char chr, Point Pos, uint8_t Scale)
    {
        char* Glyph = CurrentFont->glyphBuffer + (chr * CurrentFont->PSF_header->charsize);

        for (uint64_t y = 0; y < 16 * Scale; y++)
        {
            for (uint64_t x = 0; x < 8 * Scale; x++)
            {
                if ((*Glyph & (0b10000000 >> x / Scale)) > 0)
                {
                    *(ARGB*)((uint64_t)Backbuffer.Base + (x + Pos.X) * 4 + (y + Pos.Y) * Backbuffer.PixelsPerScanline * 4) = Foreground;
                }
                else
                {
                    *(ARGB*)((uint64_t)Backbuffer.Base + (x + Pos.X) * 4 + (y + Pos.Y) * Backbuffer.PixelsPerScanline * 4) = Background;
                }
            }
            if (y % Scale == 0)
            {
                Glyph++;
            }
        }
    }

    void Print(const char* str, uint8_t Scale)
    {
        for (int i = 0; i < cstr::Length(str); i++)
        {
            if (str[i] == '\033')
            {
                if (i + 11 > cstr::Length(str))
                {
                    return;
                }

                i++;
                switch (str[i])
                {
                case 'F':
                {
                    Foreground = ARGB(255, (str[i + 1] - '0') * 100 + (str[i + 2] - '0') * 10 + (str[i + 3] - '0'), 
                    (str[i + 4] - '0') * 100 + (str[i + 5] - '0') * 10 + (str[i + 6] - '0'), 
                    (str[i + 7] - '0') * 100 + (str[i + 8] - '0') * 10 + (str[i + 9] - '0'));
                }
                break;
                case 'B':
                {
                    Background = ARGB(255, (str[i + 1] - '0') * 100 + (str[i + 2] - '0') * 10 + (str[i + 3] - '0'), 
                    (str[i + 4] - '0') * 100 + (str[i + 5] - '0') * 10 + (str[i + 6] - '0'), 
                    (str[i + 7] - '0') * 100 + (str[i + 8] - '0') * 10 + (str[i + 9] - '0'));
                }
                break;
                }
                i += 10;
            }

            Print(str[i], Scale);
        }
    }

    void Print(char Chr, uint8_t Scale)
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
            if (CursorPos.X + 16 * Scale > Backbuffer.Width)
            {
                CursorPos.X = 0;
                CursorPos.Y += 16 * Scale;
            }
            if (CursorPos.Y + 16 > Backbuffer.Height)
            {
                ScrollUp(32);
                CursorPos.Y -= 32;
            }

            PutChar(Chr, CursorPos, Scale);
            CursorPos.X += 8 * Scale;
        }
    }

    void ScrollUp(uint64_t Amount)
    {
        uint64_t Offset = Backbuffer.PixelsPerScanline * Amount;
        Memory::Copy(Backbuffer.Base + Offset, Backbuffer.Base, Backbuffer.Size - Offset * 4);
        Memory::Set(Backbuffer.Base + Backbuffer.PixelsPerScanline * (Backbuffer.Height - Amount), 0, Offset * 4);
    }

    void SwapBuffers()
    {
        Memory::Copy(Backbuffer.Base, Frontbuffer->Base, Backbuffer.Size);

        if (DrawMouse)
        {
            for (int Y = 0; Y < 16; Y++)
            {
                for (int X = 0; X < 16; X++)
                {
                    if (X + Y < 12)
                    {
                        PutPixelFront(Point(Mouse::Position.X + X, Mouse::Position.Y + Y), ARGB(255));
                    }
                }
            }
        }
    }

    void Clear(ARGB Color)
    {
        CursorPos.X = 0;
        CursorPos.Y = 0;

        Memory::Set(Backbuffer.Base, 0, Backbuffer.Size);
    }

    Point GetScreenSize()
    {
        return Point(Backbuffer.Width, Backbuffer.Height);
    }
}
