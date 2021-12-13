#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);
	Renderer::Clear(ARGB(255, 0, 0, 255));

	InitGDT();
	IDT::SetupInterrupts();

	Renderer::Print("Hello, World!\n\r", ARGB(255));

	Renderer::SwapBuffers();

	//int* Test = (int*)0xFFFFFFFFFFFFFF;
	//*Test = 12;

	while(true)
	{
		asm("HLT");
	}
}
