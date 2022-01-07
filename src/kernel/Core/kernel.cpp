#include "kernel.h"

#include "STL/List/List.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	InitGDT();

	PageAllocator::Init(BootInfo->MemoryMap, BootInfo->ScreenBuffer, BootInfo->PSFFont);
	PageTableManager::Init(BootInfo->ScreenBuffer);

	Heap::Init();

	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSFFont);
	Renderer::Clear(STL::ARGB(0));

	PIT::SetFrequency(100);

	RTC::Update();

	IDT::SetupInterrupts();
	
	KeyBoard::Clear();
	
	ProcessHandler::Loop();

	while(true)
	{
		asm("HLT");
	}
}
