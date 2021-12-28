#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSFFont);

	InitGDT();

	PageAllocator::Init(BootInfo->MemoryMap, BootInfo->ScreenBuffer, BootInfo->PSFFont);
	PageTableManager::Init(BootInfo->ScreenBuffer);

	Renderer::Clear(ARGB(255, 0, 0, 255));

	PIT::SetFrequency(100);

	IDT::SetupInterrupts();
	
	KeyBoard::Clear();

	RTC::Update();
	
	Renderer::Print("\n\rHello, World!\n\r");
	Renderer::Print("Current Time: ");
	Renderer::Print(cstr::ToString(RTC::GetHour()));
	Renderer::Print(" : ");
	Renderer::Print(cstr::ToString(RTC::GetMinute()));
	Renderer::Print(" : ");	
	Renderer::Print(cstr::ToString(RTC::GetSecond()));
	Renderer::Print("\n\r");
	Renderer::Print("Free Memory: ");
	Renderer::Print(cstr::ToString(PageAllocator::GetFreeMemory() / 1048576));
	Renderer::Print(" MB\n\r");
	Renderer::Print("Locked Memory: ");
	Renderer::Print(cstr::ToString(PageAllocator::GetLockedMemory() / 1048576));
	Renderer::Print(" MB\n\r");

	for (int i = 0; i < 10; i++)
	{
		Renderer::Print("Random Page ");
		Renderer::Print(cstr::ToString(i));
		Renderer::Print(": ");
		Renderer::Print(cstr::ToString((uint64_t)PageAllocator::RequestPage()));
		Renderer::Print("\n\r");
	}
	
	Point ScreenSize = Renderer::GetScreenSize();
	Point OldMousePos = Mouse::Position;
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

		if (KeyBoard::IsPressed('a'))
		{
			Mouse::Draw = !Mouse::Draw;
		}

        for (int y = 0; y < 6; y++)
		{		
			Renderer::CursorPos = Point(ScreenSize.X / 2 - (21 * 8 * 3) / 2, ScreenSize.Y / 2  - (6 * 16 * 2) / 2 + 32 * y);
			for (int x = 0; x < 21; x++)
			{
				char Char = x + y * 21;
				if (KeyBoard::IsHeld(Char))
				{
					Renderer::PutChar(Char, ARGB(255, 255, 0, 0), Point(ScreenSize.X / 2 - (21 * 8 * 3) / 2 + x * 8 * 3, ScreenSize.Y / 2 - (6 * 16 * 2) + 32 * y));
				}
				else 
				{
					Renderer::PutChar(Char, ARGB(255, 0, 255, 0), Point(ScreenSize.X / 2 - (21 * 8 * 3) / 2 + x * 8 * 3, ScreenSize.Y / 2 - (6 * 16 * 2) + 32 * y));
				}
			}
		}
	}

	while(true)
	{
		asm("HLT");
	}
}
