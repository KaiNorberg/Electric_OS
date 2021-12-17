#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);
	Renderer::Clear(ARGB(255, 0, 0, 255));

	InitGDT();

	IDT::SetupInterrupts();

	Renderer::Print("Hello, World!\n\r", ARGB(255));

	Renderer::SwapBuffers();

	while (true)
	{
		Renderer::PutChar('M', ARGB(255, 255, 0, 0), Mouse::GetPosition());
        Renderer::SwapBuffers();
	}

	while(true)
	{
		asm("HLT");
	}
}
