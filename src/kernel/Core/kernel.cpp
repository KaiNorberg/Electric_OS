#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	InitGDT();

	PageAllocator::Init(BootInfo->MemoryMap, BootInfo->ScreenBuffer, BootInfo->PSFFont);
	PageTableManager::Init(BootInfo->ScreenBuffer);

	Heap::Init();

	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSFFont);

	STL::SetFont(BootInfo->PSFFont);

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
