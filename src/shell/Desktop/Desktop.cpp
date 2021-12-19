#include "Desktop.h"

#include "../Draw/Draw.h"
#include "../System/System.h"

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

        Clear(RGBA(0, 0, 0, 255));

        int64_t X = 0;
        int64_t Y = 0;
        while (true)
        {
            for (int i = 0; i < 10 + X / 100; i++)
            {
                if (X < ScreenBuffer->Width / 2)
                {
                    X++;
                }
                if (Y < ScreenBuffer->Height / 2)
                {
                    Y++;
                }

                Draw::ThinRectangle(ScreenBuffer, Point(ScreenBuffer->Width / 2 - X, ScreenBuffer->Height / 2 - Y), 
                Point(ScreenBuffer->Width / 2 + X, ScreenBuffer->Height / 2 + Y), RGBA(0, 0, 255, 255));
            }
            
            System::Sleep(1.0 / 60.0);
        }
    }

    void MainLoop()
    {
        while (true)
        {
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