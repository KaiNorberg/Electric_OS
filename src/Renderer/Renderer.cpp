#include "Renderer.h"

#include "STL/String/cstr.h"

#include "PIT/PIT.h"
#include "Memory/Heap.h"
#include "Input/Mouse.h"

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
    
    void Init(STL::Framebuffer* Screenbuffer)
    {
        Frontbuffer = Screenbuffer;
        Backbuffer = *Frontbuffer;
        Backbuffer.Base = (STL::ARGB*)Heap::Allocate(Backbuffer.Size);

        CursorPos.X = 0;
        CursorPos.Y = 0;

        Background = STL::ARGB(0);
        Foreground = STL::ARGB(255);

        DrawMouse = false;
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
                    BeforeCursor[X + Y * 16] = Backbuffer.GetPixel(STL::Point(MousePos.X + X,MousePos.Y + Y));
                    Backbuffer.PutPixel(STL::Point(MousePos.X + X, MousePos.Y + Y), STL::ARGB(255));
                }
            }

            OldMousePos = MousePos;

            STL::CopyMemory(Backbuffer.Base, Frontbuffer->Base, Backbuffer.Size);

            for (int Y = 0; Y < 12; Y++)
            {
                for (int X = 0; X < 12 - Y; X++)
                {
                    Backbuffer.PutPixel(STL::Point(MousePos.X + X, MousePos.Y + Y), BeforeCursor[X + Y * 16]);
                }
            }
            DrawMouse = true;
        }
        else
        {
            STL::CopyMemory(Backbuffer.Base, Frontbuffer->Base, Backbuffer.Size);
        }
    }

    STL::Point GetScreenSize()
    {
        return STL::Point(Backbuffer.Width, Backbuffer.Height);
    }
}
