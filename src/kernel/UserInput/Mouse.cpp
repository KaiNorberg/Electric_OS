#include "Mouse.h"

#include <stdint.h>

#include "../IO/IO.h"
#include "../Renderer/Renderer.h"
#include "../Math/Math.h"

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000010
#define PS2Rightbutton 0b00000100
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

namespace Mouse
{ 
    Point Position;

    bool Draw = false;
    bool Cleared = false;

    ARGB BeforeBuffer[256];

    void MouseWait()
    {   
        uint64_t TimeOut = 100000;

        while (TimeOut--)
        {
            if ((IO::InByte(0x64) & 0b10) == 0)
            {
                return;
            }
        }
    }

    void MouseWaitInput()
    {   
        uint64_t TimeOut = 100000;

        while (TimeOut--)
        {
            if (IO::InByte(0x64) & 0b1)
            {
                return;
            }
        }
    }

    void MouseWrite(uint8_t Value)
    {
        MouseWait();
        IO::OutByte(0x64, 0xD4);
        MouseWait();
        IO::OutByte(0x60, Value);
    }

    uint8_t MouseRead()
    {
        MouseWaitInput();
        return IO::InByte(0x60);
    }

    void InitPS2()
    {
        Position.X = 500;
        Position.Y = 500;
        Draw = false;
        Cleared = false;

        IO::OutByte(0x64, 0xA8);
        MouseWait();

        IO::OutByte(0x64, 0x20);
        MouseWaitInput();

        uint8_t Status = IO::InByte(0x60);
        Status != 0b10;
        MouseWait();
        IO::OutByte(0x64, 0x60);
        MouseWait();
        IO::OutByte(0x60, Status);

        MouseWrite(0xF6);
        MouseRead();

        MouseWrite(0xF4);
        MouseRead();
        
        for (int Y = 0; Y < 16; Y++)
        {
            for (int X = 0; X < 16; X++)
            {                            
                BeforeBuffer[X + Y * 16] = Renderer::GetPixel(Point(Position.X + X, Position.Y + Y));
            }
        }
    }
    
    void HandleMouseData(uint8_t Data)
    {
        static uint8_t MouseCycle = 0;
        static uint8_t MousePacket[4];
        
        switch(MouseCycle)
        {
        case 0:
        {
            if (((Data & 0b00001000) == 0))
            {
                break;
            }
            MousePacket[0] = Data;
            MouseCycle++;
        }
        break;
        case 1:
        {
            MousePacket[1] = Data;
            MouseCycle++;
        }
        break;
        case 2:
        {
            MousePacket[2] = Data;
            MouseCycle = 0;

            if (!Cleared)
            {
                for (int Y = 0; Y < 16; Y++)
                {
                    for (int X = 0; X < 16; X++)
                    {                              
                        if (X + Y < 12)
                        {
                            Renderer::PutPixel(Point(Position.X + X, Position.Y + Y), BeforeBuffer[X + Y * 16]);
                        }
                    }
                }
                Cleared = true;
            }

            if (MousePacket[0] & PS2XSign)
            {      
                MousePacket[1] = 256 - MousePacket[1];
                Position.X -= MousePacket[1];
            } 
            else
            {
                Position.X += MousePacket[1];
            }

            if (MousePacket[0] & PS2YSign)
            {
                MousePacket[2] = 256 - MousePacket[2];
                Position.Y += MousePacket[2];
            }
            else
            {
                Position.Y -= MousePacket[2];
            }

            Position.X = Math::Clamp(Position.X, 0, Renderer::GetScreenSize().X - 8);
            Position.Y = Math::Clamp(Position.Y, 0, Renderer::GetScreenSize().Y - 16);
            
            if (Draw)
            {
                for (int Y = 0; Y < 16; Y++)
                {
                    for (int X = 0; X < 16; X++)
                    {
                        if (X + Y < 12)
                        {
                            BeforeBuffer[X + Y * 16] = Renderer::GetPixel(Point(Position.X + X, Position.Y + Y));
                            Renderer::PutPixel(Point(Position.X + X, Position.Y + Y), ARGB(255));
                        }
                    }
                }
                Cleared = false;
            }
        }
        break;
        }
    }
}