#include "STL/String/cstr.h"
#include "STL/Graphics/Graphics.h"

#include "kernel/Renderer/Renderer.h"
#include "kernel/Interrupts/IDT.h"
#include "kernel/Memory/GDT/GDT.h"
#include "kernel/Memory/Heap.h"
#include "kernel/Memory/Paging/PageAllocator.h"
#include "kernel/Memory/Paging/PageTable.h"
#include "kernel/Input/KeyBoard.h"
#include "kernel/Input/Mouse.h"
#include "kernel/PIT/PIT.h"
#include "kernel/RTC/RTC.h"
#include "kernel/Debug/Debug.h"
#include "kernel/System/System.h"
#include "kernel/ProcessHandler/ProcessHandler.h"
/// <summary>
/// Compiler definitions.
/// </summary>
extern "C" void __stack_chk_fail(void);
extern "C" void* __dso_handle;
extern "C" void* __cxa_atexit;

/// <summary>
/// The struct passed to the kernel from the bootloader.
/// </summary>
struct BootLoaderInfo
{
	STL::Framebuffer* ScreenBuffer;
	STL::PSF_FONT * PSFFont;
	EFI_MEMORY_MAP* MemoryMap;
};

/// <summary>
/// The first and last address used by the kernel.
/// </summary>
extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
