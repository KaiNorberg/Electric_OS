#include "kernel.h"

/// <summary>
/// Compiler definitions.
/// </summary>
void __stack_chk_fail(void)
{

}
void* __dso_handle = (void*) &__dso_handle;
void* __cxa_atexit = (void*) &__cxa_atexit;

/// <summary>
////The function called by the bootloader when it leaves boot services.
/// </summary>
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
		
	ACPI::Init(BootInfo->RSDP);

	PCI::ResetEnumeration();

	ProcessHandler::Loop();

	while(true)
	{
		asm("HLT");
	}
}
