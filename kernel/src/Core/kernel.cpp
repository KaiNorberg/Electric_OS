#include "kernel.h"

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	GDTDesc GDTDescriptor;
	GDTDescriptor.Size = sizeof(GDT) - 1;
	GDTDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&GDTDescriptor);

	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);
	PageFrameAllocator::Init(BootInfo->MemoryMap, BootInfo->MemoryMapSize, BootInfo->MemoryMapDescSize);
	PageFrameAllocator::LockPages(&_KernelStart, ((uint64_t)&_KernelEnd - (uint64_t)&_KernelStart) / 4096 + 1);
	PageFrameAllocator::LockPages(BootInfo->ScreenBuffer->Base, BootInfo->ScreenBuffer->Size / 0x1000 + 1);
	PageTableManager::Init(BootInfo->ScreenBuffer);

	IDT::SetupInterrupts();

	Renderer::Clear();
	Renderer::Print("Hello, World!\n\r");

	//int* Test = (int*)0xFFFFFFFFFFFFFF;
	//*Test = 12;

	while(true)
	{
		asm("hlt");
	}
}
