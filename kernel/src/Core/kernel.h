#include "../Rendering/Renderer.h"
#include "../Interrupts/IDT.h"
#include "../Memory/Paging/PageFrameAllocator.h"
#include "../Memory/Paging/PageMapIndexer.h"
#include "../Memory/Paging/PageTableManager.h"
#include "../Memory/Paging/Paging.h"
#include "../Memory/GDT/GDT.h"
#include "../Memory/BitMap.h"
#include "../Memory/Memory.h"
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
