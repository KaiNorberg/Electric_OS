#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	InitGDT();

	PageAllocator::Init(BootInfo->MemoryMap, BootInfo->ScreenBuffer, BootInfo->PSFFont);
	PageTableManager::Init(BootInfo->ScreenBuffer);

	Heap::Init();

	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSFFont);
	Renderer::Clear(ARGB(0));

	PIT::SetFrequency(100);

	RTC::Update();

	IDT::SetupInterrupts();
	
	KeyBoard::Clear();
	
	SystemCalls::Args[0] = 0;
	SystemCalls::Args[1] = (uint64_t)"panic";
	asm("int $0x80");
	Renderer::Print((char*)SystemCalls::Args[1]);
	Renderer::SwapBuffers();

	//tty::tty();

	while(true)
	{
		asm("HLT");
	}
}
