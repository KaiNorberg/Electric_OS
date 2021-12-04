#include "../Rendering/Renderer.h"
#include "../Memory/Memory.h"
#include "../Memory/BitMap.h"
#include "../String/cstr.h"

struct BootInfo
{
	Framebuffer* framebuffer;
	PSF1_FONT* psf1_Font;
	EFI_MEMORY_DESCRIPTOR* MemoryMap;
	uint64_t MemoryMapSize;
	uint64_t MemoryMapDescSize;
};

extern "C" void _start(BootInfo* bootInfo)
{
	Renderer::Init(bootInfo->framebuffer, bootInfo->psf1_Font);

	Renderer::Print("Hello, World!");

    return;
}
