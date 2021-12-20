#include "../Rendering/Renderer.h"
#include "../Interrupts/IDT.h"
#include "../Memory/GDT/GDT.h"
#include "../Memory/Memory.h"
#include "../String/cstr.h"
#include "../UserInput/KeyBoard.h"
#include "../UserInput/Mouse.h"
#include "../PIT/PIT.h"

struct BootLoaderInfo
{
	Framebuffer* ScreenBuffer;
	PSF1_FONT* PSF1Font;
	EFI_MEMORY_DESCRIPTOR* MemoryMap;
	uint64_t MemoryMapSize;
	uint64_t MemoryMapDescSize;
	void* ShellEntry;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
