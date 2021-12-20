#include "Desktop.h"

#include "../Draw/Draw.h"
#include "../System/System.h"

namespace Desktop
{
    Framebuffer* ScreenBuffer;
    Point OldCursorPos;
    
    uint8_t CursorBitMap[] =
    {
        0b11111111, 0b11100000, 
        0b11111111, 0b10000000, 
        0b11111110, 0b00000000, 
        0b11111100, 0b00000000, 
        0b11111000, 0b00000000, 
        0b11110000, 0b00000000, 
        0b11100000, 0b00000000, 
        0b11000000, 0b00000000, 
        0b11000000, 0b00000000, 
        0b10000000, 0b00000000, 
        0b10000000, 0b00000000, 
        0b00000000, 0b00000000, 
        0b00000000, 0b00000000, 
        0b00000000, 0b00000000, 
        0b00000000, 0b00000000, 
        0b00000000, 0b00000000
    };

    RGBA GetPixel(Point Pos)
    {
        return *(RGBA*)((uint64_t)ScreenBuffer->Base + Pos.X * 4 + Pos.Y * ScreenBuffer->PixelsPerScanline * 4);
    }

    void DrawCursor()
    {
        Point CursorPos = System::GetMousePos();

        for (int Y = 0; Y < 16; Y++)
        {
            for (int X = 0; X < 16; X++)
            {
                int Bit = Y * 16 + X;
                int Byte = Bit / 8;

                if ((CursorBitMap[Byte] & (0b10000000 >> (X % 8))))
                {
                    if (GetPixel(Point(OldCursorPos.X + X, OldCursorPos.Y + Y)).ToInt() != RGBA(0, 0, 255, 255).ToInt())
                    {
                        Draw::Pixel(ScreenBuffer, Point(OldCursorPos.X + X, OldCursorPos.Y + Y), RGBA(0, 0, 255, 255));
                    }
                    if (GetPixel(Point(CursorPos.X + X, CursorPos.Y + Y)).ToInt() != RGBA(255).ToInt())
                    {
                        Draw::Pixel(ScreenBuffer, Point(CursorPos.X + X, CursorPos.Y + Y), RGBA(255));
                    }
                }
            }
        }

        OldCursorPos = CursorPos;
    }

    void Init(Framebuffer* Buffer)
    {        
        bool Initialized = false;
        if (Initialized)
        {
            return;
        }
        Initialized = true;

        ScreenBuffer = Buffer;

        Clear(RGBA(0, 0, 0, 255));

        for (int i = 0; i < ScreenBuffer->Width / 1.3; i += 1 + i / 20)
        {
            Draw::ThinCircle(ScreenBuffer, Point(ScreenBuffer->Width / 2, ScreenBuffer->Height / 2), RGBA(0, 0, 255, 255), i, 2 + i  / 20);

            System::Sleep(1.0 / 60.0);
        }
    }

    void MainLoop()
    {
        while (true)
        {
            DrawCursor();
            asm("HLT");
        }
    }

    void Clear(RGBA Color)
    {
        for (int Y = 0; Y < ScreenBuffer->Height; Y++)
        {
            for (int X = 0; X < ScreenBuffer->Width; X++)
            {
                *(uint32_t*)((uint64_t)ScreenBuffer->Base + X * 4 + Y * ScreenBuffer->PixelsPerScanline * 4) = Color.ToInt();
            }
        }
    }
}