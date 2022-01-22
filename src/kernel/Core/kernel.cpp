#include "kernel.h"

extern "C" void __stack_chk_fail(void)
{

}
extern "C" void* __dso_handle = (void*) &__dso_handle;
extern "C" void* __cxa_atexit = (void*) &__cxa_atexit;

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
