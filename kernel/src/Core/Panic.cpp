#include "Panic.h"
#include "../Rendering/Renderer.h"

void KernelPanic(const char* Message)
{
    asm("cli");

    Renderer::Clear(ARGB(255, 255, 0, 0));
    Renderer::Print("KERNEL PANIC!\n\r");
    Renderer::Print("UNEXPECTED ERROR OCCURRED\n\r");
    Renderer::Print("ERROR RETURNED FOLLOWING MESSAGE: ");
    Renderer::Print(Message);
    Renderer::SwapBuffers();

    asm("HLT");
}
