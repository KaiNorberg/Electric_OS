#include "Desktop.h"

namespace Desktop
{
    Framebuffer* ScreenBuffer;

    void Init(Framebuffer* Buffer)
    {        
        bool Initialized = false;
        if (Initialized)
        {
            return;
        }
        Initialized = true;

        ScreenBuffer = Buffer;

        Clear(RGBA(0, 0, 255, 255));
    }

    void MainLoop()
    {
        while (true)
        {
            asm("HLT");
        }
    }

    void SetPixel(Point Pos, RGBA Color)
    {
        *(uint64_t*)((uint64_t)ScreenBuffer->Base + Pos.X * 4 + Pos.Y * ScreenBuffer->PixelsPerScanline * 4) = Color.ToInt();
    }

    void Clear(RGBA Color)
    {
        for (int Y = 0; Y < ScreenBuffer->Height; Y++)
        {
            for (int X = 0; X < ScreenBuffer->Width; X++)
            {
                SetPixel(Point(X, Y), Color);
            }
        }
    }
}