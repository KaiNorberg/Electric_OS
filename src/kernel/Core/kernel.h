#include "../Rendering/Renderer.h"
#include "../Interrupts/IDT.h"
#include "../Memory/GDT/GDT.h"
#include "../Memory/Memory.h"
#include "../Memory/Pageing/PageAllocator.h"
#include "../Memory/Pageing/PageTable.h"
#include "../String/cstr.h"
#include "../UserInput/KeyBoard.h"
#include "../UserInput/Mouse.h"
#include "../PIT/PIT.h"
#include "../RTC/RTC.h"
#include "../tty/tty.h"

struct BootLoaderInfo
{
	Framebuffer* ScreenBuffer;
	PSF_FONT* PSFFont;
	EFI_MEMORY_MAP* MemoryMap;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
