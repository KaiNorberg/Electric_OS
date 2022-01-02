#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSFFont);

	InitGDT();

	PageAllocator::Init(BootInfo->MemoryMap, BootInfo->ScreenBuffer, BootInfo->PSFFont);
	PageTableManager::Init(BootInfo->ScreenBuffer);

	Renderer::Clear(ARGB(0));

	PIT::SetFrequency(100);

	RTC::Update();

	IDT::SetupInterrupts();
	
	KeyBoard::Clear();

	tty::tty();

	while(true)
	{
		asm("HLT");
	}
}
