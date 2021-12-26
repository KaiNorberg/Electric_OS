#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSFFont);
	Renderer::Clear(ARGB(255, 0, 0, 255));

	InitGDT();

	PageAllocator::Init(BootInfo->MemoryMap);

	PIT::SetFrequency(100);

	IDT::SetupInterrupts();
	
	Renderer::Print("Hello, World!\n\r");
	Renderer::Print("Total Memory: ");
	Renderer::Print(cstr::ToString(PageAllocator::GetTotalMemory() / 1048576));
	Renderer::Print(" MB\n\r");
	Renderer::Print("Free Memory: ");
	Renderer::Print(cstr::ToString(PageAllocator::GetFreeMemory() / 1048576));
	Renderer::Print(" MB\n\r");
	Renderer::Print("Locked Memory: ");
	Renderer::Print(cstr::ToString(PageAllocator::GetLockedMemory() / 1048576));
	Renderer::Print(" MB\n\r");
	Renderer::Print("Reserved Memory: ");
	Renderer::Print(cstr::ToString(PageAllocator::GetReservedMemory() / 1048576));
	Renderer::Print(" MB\n\r");

	for (int i = 0; i < 10; i++)
	{
		Renderer::Print("Random Page ");
		Renderer::Print(cstr::ToString(i));
		Renderer::Print(": ");
		Renderer::Print(cstr::ToString((uint64_t)PageAllocator::RequestPage()));
		Renderer::Print("\n\r");
	}

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
	}

	while(true)
	{
		asm("HLT");
	}
}
