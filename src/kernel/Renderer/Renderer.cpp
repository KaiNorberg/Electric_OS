#include "Renderer.h"

#include "STL/String/cstr.h"

#include "kernel/PIT/PIT.h"
#include "kernel/Memory/Heap.h"
#include "kernel/Input/Mouse.h"

#include <stdint.h>

namespace Renderer
{
    STL::Framebuffer* Frontbuffer;
    STL::Framebuffer Backbuffer;

    STL::PSF_FONT* CurrentFont;

    STL::Point CursorPos;

    STL::ARGB Background;
    STL::ARGB Foreground;

    bool DrawMouse; 
    STL::Point OldMousePos;
    STL::ARGB BeforeCursor[16 * 16];

    void Init(STL::Framebuffer* framebuffer, STL::PSF_FONT * PSF_Font)
    {
        Frontbuffer = framebuffer;
        Backbuffer = *Frontbuffer;
        Backbuffer.Base = (STL::ARGB*)Heap::Allocate(Backbuffer.Size);

        CurrentFont = PSF_Font;

        CursorPos.X = 0;
        CursorPos.Y = 0;

        Background = STL::ARGB(0);
        Foreground = STL::ARGB(255);

        DrawMouse = false;
    }

    void PutPixel(STL::Point Pixel, STL::ARGB Color)
    {
        if (Pixel.X > Backbuffer.Width || Pixel.X < 0 || Pixel.Y > Backbuffer.Height || Pixel.Y < 0)
        {
            return;
        }

        *(STL::ARGB*)((uint64_t)Backbuffer.Base + Pixel.X * 4 + Pixel.Y * Backbuffer.PixelsPerScanline * 4) = Color;
    }

    STL::ARGB GetPixel(STL::Point Pixel)
    {        
        if (Pixel.X > Backbuffer.Width || Pixel.X < 0 || Pixel.Y > Backbuffer.Height || Pixel.Y < 0)
        {
            return STL::ARGB(0);
        }

        return *(STL::ARGB*)((uint64_t)Backbuffer.Base + Pixel.X * 4 + Pixel.Y * Backbuffer.PixelsPerScanline * 4);
    }

    void PutChar(char chr, STL::Point Pos, uint8_t Scale)
    {
        char* Glyph = CurrentFont->glyphBuffer + (chr * CurrentFont->PSF_header->charsize);

        for (uint64_t y = 0; y < 16 * Scale; y++)
        {
            for (uint64_t x = 0; x < 8 * Scale; x++)
            {
                if ((*Glyph & (0b10000000 >> x / Scale)) > 0)
                {
                    *(STL::ARGB*)((uint64_t)Backbuffer.Base + (x + Pos.X) * 4 + (y + Pos.Y) * Backbuffer.PixelsPerScanline * 4) = Foreground;
                }
                else
                {
                    *(STL::ARGB*)((uint64_t)Backbuffer.Base + (x + Pos.X) * 4 + (y + Pos.Y) * Backbuffer.PixelsPerScanline * 4) = Background;
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
        for (int i = 0; i < STL::Length(str); i++)
        {
            if (str[i] == '\033')
            {
                if (i + 11 > STL::Length(str))
                {
                    return;
                }

                i++;
                switch (str[i])
                {
                case 'F':
                {
                    Foreground = STL::ARGB(255, (str[i + 1] - '0') * 100 + (str[i + 2] - '0') * 10 + (str[i + 3] - '0'), 
                    (str[i + 4] - '0') * 100 + (str[i + 5] - '0') * 10 + (str[i + 6] - '0'), 
                    (str[i + 7] - '0') * 100 + (str[i + 8] - '0') * 10 + (str[i + 9] - '0'));
                }
                break;
                case 'B':
                {
                    Background = STL::ARGB(255, (str[i + 1] - '0') * 100 + (str[i + 2] - '0') * 10 + (str[i + 3] - '0'), 
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
        STL::CopyMemory(Backbuffer.Base + Offset, Backbuffer.Base, Backbuffer.Size - Offset * 4);
        STL::SetMemory(Backbuffer.Base + Backbuffer.PixelsPerScanline * (Backbuffer.Height - Amount), 0, Offset * 4);
    }

    void RedrawMouse()
    {
        if (DrawMouse)
        {            
            for (int Y = 0; Y < 16; Y++)
            {
                for (int X = 0; X < 16; X++)
                {
                    if (X + Y < 12)
                    {
                        if (GetPixel(STL::Point(OldMousePos.X + X, OldMousePos.Y + Y)).ToInt() == STL::ARGB(255).ToInt())
                        {
                            PutPixel(STL::Point(OldMousePos.X + X, OldMousePos.Y + Y), BeforeCursor[X + Y * 16]);
                        }
                    }
                }
            }

            for (int Y = 0; Y < 16; Y++)
            {
                for (int X = 0; X < 16; X++)
                {
                    if (X + Y < 12)
                    {
                        BeforeCursor[X + Y * 16] = GetPixel(STL::Point(Mouse::Position.X + X, Mouse::Position.Y + Y));
                        PutPixel(STL::Point(Mouse::Position.X + X, Mouse::Position.Y + Y), STL::ARGB(255));
                    }
                }
            }
            
            for (int y = OldMousePos.Y; y < OldMousePos.Y + 16; y++)
            {
                for (int x = OldMousePos.X; x < OldMousePos.X + 16; x++)
                {
                    *(STL::ARGB*)((uint64_t)Renderer::Frontbuffer->Base + x * 4 + y * Renderer::Frontbuffer->PixelsPerScanline * 4) = 
                    *(STL::ARGB*)((uint64_t)Renderer::Backbuffer.Base + x * 4 + y * Renderer::Backbuffer.PixelsPerScanline * 4);
                }
            }

            OldMousePos = Mouse::Position;

            for (int y = Mouse::Position.Y; y < Mouse::Position.Y + 16; y++)
            {
                for (int x = Mouse::Position.X; x < Mouse::Position.X + 16; x++)
                {
                    *(STL::ARGB*)((uint64_t)Renderer::Frontbuffer->Base + x * 4 + y * Renderer::Frontbuffer->PixelsPerScanline * 4) = 
                    *(STL::ARGB*)((uint64_t)Renderer::Backbuffer.Base + x * 4 + y * Renderer::Backbuffer.PixelsPerScanline * 4);
                }
            }
        }  
    }

    void SwapBuffers()
    {
        STL::CopyMemory(Backbuffer.Base, Frontbuffer->Base, Backbuffer.Size);
    }
    
    void Clear()
    {
        CursorPos.X = 0;
        CursorPos.Y = 0;

        STL::SetMemory(Backbuffer.Base, 0, Backbuffer.Size);
    }

    STL::Point GetScreenSize()
    {
        return STL::Point(Backbuffer.Width, Backbuffer.Height);
    }
}
