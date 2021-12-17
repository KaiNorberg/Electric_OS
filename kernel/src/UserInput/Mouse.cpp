#include "Mouse.h"

#include <stdint.h>

#include "../IO/IO.h"
#include "../Rendering/Renderer.h"
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
    }
    
    uint8_t MouseCycle = 0;
    uint8_t MousePacket[4];
    void HandleMouseData(uint8_t Data)
    {
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
        }
        break;
        }
    }

    Point GetPosition()
    {
        return Position;
    }
}