#include "shell.h"
#include "../System/System.h"
#include "../Rendering/Framebuffer.h"
#include "../Rendering/Font.h"
#include "../Desktop/Desktop.h"

struct KernelInfo
{
	void* SysCallAddr;
	Framebuffer* ScreenBuffer;
	PSF1_FONT* PSF1Font;
};

void ShellEntry(KernelInfo* Info)
{
	System::Init(Info->SysCallAddr);

	Desktop::Init(Info->ScreenBuffer);
	Desktop::MainLoop();
}  