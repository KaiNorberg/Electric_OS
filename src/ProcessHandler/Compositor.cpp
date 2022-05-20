#include "Compositor.h"
#include "ProcessHandler.h"

#include "STL/Memory/Memory.h"

#include "Input/Mouse.h"

namespace Compositor
{    
    STL::Framebuffer* Frontbuffer;
    STL::Framebuffer Backbuffer;

    bool RedrawRequest = false;
    bool BufferSwapRequest = false;
    
    bool DrawMouse = false;

    STL::Point OldMousePos;

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

    void Init(STL::Framebuffer* Screenbuffer)
    {
        Frontbuffer = Screenbuffer;
        Backbuffer = *Frontbuffer;
        
        Backbuffer.Base = (STL::ARGB*)STL::Malloc(Backbuffer.Size);
        Backbuffer.Clear();
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

    void Update(uint32_t i)
    {
        for (uint32_t j = i + 1; j < ProcessHandler::Processes.Length(); j++)
        {
            if (ProcessHandler::Processes[i]->Contains(ProcessHandler::Processes[j]))
            {
                RedrawRequest = true;
                return;
            }
        }
        ProcessHandler::Processes[i]->Render(&Backbuffer);
        BufferSwapRequest = true;
    }

    void Update()
    {
        if (RedrawRequest)
        {        
            for (uint32_t i = 0; i < ProcessHandler::Processes.Length(); i++)
            {
                ProcessHandler::Processes[i]->Render(&Backbuffer);
            }    
            SwapBuffers();
            RedrawRequest = false;
            BufferSwapRequest = false;     
        }
        else if (BufferSwapRequest)
        {                
            SwapBuffers();
            BufferSwapRequest = false;
        }
    }
}