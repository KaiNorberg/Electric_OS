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
/// The function called by the bootloader when it leaves boot services.
/// </summary>
extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	InitGDT();
	
	PageAllocator::Init(BootInfo->MemoryMap, BootInfo->ScreenBuffer);
	PageTableManager::Init(BootInfo->ScreenBuffer);

	Heap::Init();

	STL::SetFonts(BootInfo->PSFFonts, BootInfo->FontAmount);

	Renderer::Init(BootInfo->ScreenBuffer);

	PIT::SetFrequency(100);

	RTC::Update();

	IDT::SetupInterrupts();
		
	ACPI::Init(BootInfo->RSDP);

	PCI::Init();

	AHCI::Init();

	ProcessHandler::Loop();

	while(true)
	{
		asm("HLT");
	}
}
