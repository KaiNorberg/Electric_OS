#include "Mouse.h"

#include <stdint.h>

#include "IO/IO.h"
#include "Renderer/Renderer.h"

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000010
#define PS2Rightbutton 0b00000100
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

namespace Mouse
{ 
    STL::Point Position;

    bool LeftHeld;
    
    bool MiddleHeld;
    
    bool RightHeld;

    /// <summary>
    /// Waits untill the mouse is ready to recive data or untill a timeout is reached.
    /// </summary>
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

    /// <summary>
    /// Waits untill the mouse is ready to send data or untill a timeout is reached.
    /// </summary>
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

    /// <summary>
    /// Writes a command to the mouse port (0x60).
    /// </summary>
    void MouseWrite(uint8_t Value)
    {
        MouseWait();
        IO::OutByte(0x64, 0xD4);
        MouseWait();
        IO::OutByte(0x60, Value);
    }

    /// <summary>
    /// Reads date from the mouse port (0x60).
    /// </summary>
    uint8_t MouseRead()
    {
        MouseWaitInput();
        return IO::InByte(0x60);
    }

    void InitPS2()
    {
        Position.X = 500;
        Position.Y = 500;

        IO::OutByte(0x64, 0xA8); //Enable mouse
        MouseWait();

        IO::OutByte(0x64, 0x20);
        MouseWaitInput();

        uint8_t Status = IO::InByte(0x60);
        Status |= 0b10;
        MouseWait();
        IO::OutByte(0x64, 0x60);
        MouseWait();
        IO::OutByte(0x60, Status); //Set the compaq status bit.

        MouseWrite(0xF6);
        MouseRead();

        MouseWrite(0xF4);
        MouseRead();
    }
    
    void HandleMousePacket(uint8_t* MousePacket)
    {   
        /// If sign bit is set move in negative direction.
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

        /// Check for button presses.
        if ((MousePacket[0] & PS2Leftbutton))
        {
            LeftHeld = true;
        }  
        if ((MousePacket[0] & PS2Middlebutton))
        {
            MiddleHeld = true;
        }
        if ((MousePacket[0] & PS2Rightbutton))
        {
            RightHeld = true;
        }

        /// Clamp mouse pos to the screen.
        Position.X = STL::Clamp(Position.X, 0, Renderer::GetScreenSize().X - 8);
        Position.Y = STL::Clamp(Position.Y, 0, Renderer::GetScreenSize().Y - 16);
    }
}