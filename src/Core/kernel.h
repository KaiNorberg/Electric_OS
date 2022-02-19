#include "STL/String/cstr.h"
#include "STL/Graphics/Graphics.h"

#include "Renderer/Renderer.h"
#include "Interrupts/IDT.h"
#include "Memory/GDT/GDT.h"
#include "Memory/Heap.h"
#include "Memory/Paging/PageAllocator.h"
#include "Memory/Paging/PageTable.h"
#include "Input/KeyBoard.h"
#include "Input/Mouse.h"
#include "PIT/PIT.h"
#include "RTC/RTC.h"
#include "Debug/Debug.h"
#include "System/System.h"
#include "ProcessHandler/ProcessHandler.h"
#include "ACPI/ACPI.h"
#include "AHCI/AHCI.h"
#include "PCI/PCI.h"
#include "UEFI/UEFI.h"

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
	STL::PSF_FONT** PSFFonts;
	uint8_t FontAmount;
	EFI_MEMORY_MAP* MemoryMap;
	RSDP2* RSDP;
	EfiRuntimeServices* RT;
};

/// <summary>
/// The first and last address used by the kernel.
/// </summary>
extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;
