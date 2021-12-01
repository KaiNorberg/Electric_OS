#include "Rendering/Renderer.h"
#include "Memory/Memory.h"
#include "String/cstr.h"

struct BootInfo {
	Framebuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* MemoryMap;
	uint64_t MemoryMapSize;
	uint64_t MemoryMapDescSize;
};

extern "C" void _start(BootInfo* bootInfo)
{
	Renderer::Init(bootInfo->framebuffer, bootInfo->psf1_Font);

	uint64_t MemoryMapEntries = bootInfo->MemoryMapSize / bootInfo->MemoryMapDescSize;
	Renderer::Print("Memory size: 0x");
	Renderer::Print(cstr::to_hstring((uint64_t)Memory::GetTotalSize(bootInfo->MemoryMap, MemoryMapEntries, bootInfo->MemoryMapDescSize)));

    return;
}
