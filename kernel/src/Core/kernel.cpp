#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);
	Renderer::Clear();

	GDTDesc GDTDescriptor;
	GDTDescriptor.Size = sizeof(GDT) - 1;
	GDTDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&GDTDescriptor);

	IDT::SetupInterrupts();

	Renderer::Print("Hello, World!\n\r");
	
	while(true)
	{
		asm("HLT");
	}
}
