#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);
	Renderer::Clear(ARGB(255, 0, 0, 255));

	InitGDT();

	IDT::SetupInterrupts();

	Renderer::Print("Hello, World!\n\r", ARGB(255));

	*(SYSCALL_PANIC_ARG0) = "Hello, Test!";
	asm("int $0xFF");

	Renderer::SwapBuffers();

	while (true)
	{
		Renderer::PutChar('M', ARGB(255, 255, 0, 0), Mouse::Position);
        Renderer::SwapBuffers();
	}

	while(true)
	{
		asm("HLT");
	}
}
