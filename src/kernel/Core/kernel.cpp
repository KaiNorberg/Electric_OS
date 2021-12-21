#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);
	Renderer::Clear(ARGB(255, 0, 0, 255));

	InitGDT();

	PIT::SetFrequency(60);

	IDT::SetupInterrupts();
	
	Renderer::Print("Hello, World!\n\r");
	Renderer::Print("Memory Usage: ");
	Renderer::Print(cstr::ToString((((uint64_t)&_KernelEnd) - ((uint64_t)&_KernelStart)) / 1048576));
	Renderer::Print(" MB\n\r");

	Renderer::SwapBuffers();

	bool Alternate = false;
	uint64_t PrevTick = PIT::Ticks;
	while (true)
	{		
		if (PrevTick + PIT::GetFrequency() < PIT::Ticks)
		{
			ARGB Color;
			if (Alternate)
			{
				Color = ARGB(255, 255, 0, 0);
			}
			else
			{
				Color = ARGB(255, 0, 0, 255);
			}

			for (int Y = 1060; Y < 1080; Y++)
			{
				for (int X = 0; X < 1980; X++)
				{
					Renderer::PutPixel(Point(X, Y), Color);
				}
			}

			PrevTick = PIT::Ticks;
			Alternate = !Alternate;
		}

        uint8_t Key = KeyBoard::GetKeyPress();
        if (Key != 0)
        {
            Renderer::Print(Key);
        }
		Renderer::PutChar('M', ARGB(255, 255, 0, 0), Mouse::Position);

        Renderer::SwapBuffers();
	}

	while(true)
	{
		asm("HLT");
	}
}
