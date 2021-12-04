#include "../Rendering/Renderer.h"
#include "../Memory/Memory.h"
#include "../Memory/BitMap.h"
#include "../String/cstr.h"

struct BootLoaderInfo
{
	Framebuffer* ScreenBuffer;
	PSF1_FONT* PSF1Font;
	EFI_MEMORY_DESCRIPTOR* MemoryMap;
	uint64_t MemoryMapSize;
	uint64_t MemoryMapDescSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void KernelMain(BootLoaderInfo* BootInfo)
{
	Memory::ReadEfiMemoryMap(BootInfo->MemoryMap, BootInfo->MemoryMapSize, BootInfo->MemoryMapDescSize);
	Memory::LockPages(&_KernelStart, ((uint64_t)&_KernelEnd - (uint64_t)&_KernelStart) / 4096 + 1);
	Renderer::Init(BootInfo->ScreenBuffer, BootInfo->PSF1Font);

	Renderer::Print("Free Memory: ");
	Renderer::Print(cstr::ToString((int64_t)Memory::GetFreeMemory() / 1024));
	Renderer::Print(" KB\n\r");

	Renderer::Print("Used Memory: ");
	Renderer::Print(cstr::ToString((int64_t)Memory::GetUsedMemory() / 1024));
	Renderer::Print(" KB\n\r");

	Renderer::Print("Reserved Memory: ");
	Renderer::Print(cstr::ToString((int64_t)Memory::GetReservedMemory() / 1024));
	Renderer::Print(" KB\n\r");

	for (int i = 0; i < 20; i ++)
	{
		void* Address = Memory::RequestPage();
		Renderer::Print("Allocated Page Address: ");
		Renderer::Print(cstr::ToString((int64_t)Address));
		Renderer::Print("\n\r");
	}
    return;
}
