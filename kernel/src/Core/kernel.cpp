#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);
	Renderer::Clear(ARGB(255, 0, 0, 255));

	InitGDT();

	IDT::SetupInterrupts();

	Renderer::Print("Hello, World!\n\r");
	Renderer::Print("Memory Usage: ");
	Renderer::Print(cstr::ToString((((uint64_t)&_KernelEnd) - ((uint64_t)&_KernelStart)) / 1000));
	Renderer::Print(" KB\n\r");

	//*(SYSCALL_SELECTOR) = 0;
	//*((char**)SYSCALL_ARG0) = "TEST!";
	//asm("int $0x80");

	while (true)
	{
		Renderer::PutChar('M', ARGB(255, 255, 0, 0), Mouse::Position);
		uint8_t Key = KeyBoard::GetKeyPress();
        if (Key != 0)
        {
            Renderer::Print(Key);
        }
	}

	while(true)
	{
		asm("HLT");
	}
}
