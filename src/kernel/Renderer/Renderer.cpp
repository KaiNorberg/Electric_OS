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

    STL::Point CursorPos;

    STL::ARGB Background;
    STL::ARGB Foreground;

    bool DrawMouse; 
    STL::Point OldMousePos;
    
    void Init(STL::Framebuffer* framebuffer)
    {
        Frontbuffer = framebuffer;
        Backbuffer = *Frontbuffer;
        Backbuffer.Base = (STL::ARGB*)Heap::Allocate(Backbuffer.Size);

        CursorPos.X = 0;
        CursorPos.Y = 0;

        Background = STL::ARGB(0);
        Foreground = STL::ARGB(255);

        DrawMouse = false;
    }

    void PutPixel(STL::Point Pixel, STL::ARGB Color)
    {
        if (Pixel.X > (int32_t)Backbuffer.Width || Pixel.X < 0 || Pixel.Y > (int32_t)Backbuffer.Height || Pixel.Y < 0)
        {
            return;
        }

        *(STL::ARGB*)((uint64_t)Backbuffer.Base + Pixel.X * 4 + Pixel.Y * Backbuffer.PixelsPerScanline * 4) = Color;
    }

    STL::ARGB GetPixel(STL::Point Pixel)
    {        
        if (Pixel.X > (int32_t)Backbuffer.Width || Pixel.X < 0 || Pixel.Y > (int32_t)Backbuffer.Height || Pixel.Y < 0)
        {
            return STL::ARGB(0);
        }

        return *(STL::ARGB*)((uint64_t)Backbuffer.Base + Pixel.X * 4 + Pixel.Y * Backbuffer.PixelsPerScanline * 4);
    }

    void PutChar(char chr, STL::Point Pos, uint8_t Scale)
    {
        Backbuffer.PutChar(chr, Pos, Scale, Foreground, Background);
    }

    void Print(const char* str, uint8_t Scale)
    {
        Backbuffer.Print(str, CursorPos, Scale, Foreground, Background);
    }

    void Print(char Chr, uint8_t Scale)
    {        
        Backbuffer.PutChar(Chr, CursorPos, Scale, Foreground, Background);
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
            STL::Point MousePos = Mouse::Position;
                
            for (int Y = 0; Y < 12; Y++)
            {
                STL::CopyMemory(Backbuffer.Base + (OldMousePos.Y + Y) * Backbuffer.PixelsPerScanline + (OldMousePos.X), 
                Frontbuffer->Base + (OldMousePos.Y + Y) * Frontbuffer->PixelsPerScanline + (OldMousePos.X), (12 - Y) * 4);
            }

            OldMousePos = MousePos;

            for (int Y = 0; Y < 12; Y++)
            {              
                STL::SetMemory(Frontbuffer->Base + (MousePos.Y + Y) * Frontbuffer->PixelsPerScanline + (MousePos.X), 255, (12 - Y) * 4);
            }
        }  
    }

    void SwapBuffers()
    {             
        STL::ARGB BeforeCursor[16 * 16];
    
        if (DrawMouse)
        {
            DrawMouse = false;
            STL::Point MousePos = Mouse::Position;
            
            for (int Y = 0; Y < 12; Y++)
            {
                for (int X = 0; X < 12 - Y; X++)
                {
                    BeforeCursor[X + Y * 16] = GetPixel(STL::Point(MousePos.X + X,MousePos.Y + Y));
                    PutPixel(STL::Point(MousePos.X + X, MousePos.Y + Y), STL::ARGB(255));
                }
            }

            OldMousePos = MousePos;

            STL::CopyMemory(Backbuffer.Base, Frontbuffer->Base, Backbuffer.Size);

            for (int Y = 0; Y < 12; Y++)
            {
                for (int X = 0; X < 12 - Y; X++)
                {
                    PutPixel(STL::Point(MousePos.X + X, MousePos.Y + Y), BeforeCursor[X + Y * 16]);
                }
            }
            DrawMouse = true;
        }
        else
        {
            STL::CopyMemory(Backbuffer.Base, Frontbuffer->Base, Backbuffer.Size);
        }
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
