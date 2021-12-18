#include "kernel.h"

struct KernelInfo
{
	void* SysCallAddr;
	Framebuffer* ScreenBuffer;
	PSF1_FONT* PSF1Font;
};

void ShellEntry(KernelInfo* Info);

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);
	Renderer::Clear(ARGB(255, 0, 0, 0));

	InitGDT();

	IDT::SetupInterrupts();

	Renderer::Print("Hello, World!\n\r");
	Renderer::Print("Memory Usage: ");
	Renderer::Print(cstr::ToString((((uint64_t)&_KernelEnd) - ((uint64_t)&_KernelStart)) / 1000));
	Renderer::Print(" KB\n\r");

	KernelInfo Info;
	Info.SysCallAddr = (void*)&(_SYS_CALL_ADDR);
	Info.ScreenBuffer = BootInfo->ScreenBuffer;
	Info.PSF1Font = BootInfo->PSF1Font;

	ShellEntry(&Info);
}
