#include "Panic.h"
#include "../Rendering/Renderer.h"

void Panic(const char* Message)
{
    Renderer::Clear(0x00FF0000);
    Renderer::Print("KERNEL PANIC!\n\r");
    Renderer::Print("UNEXPECTED ERROR OCCURRED\n\r");
    Renderer::Print("ERROR RETURNED FOLLOWING MESSAGE: ");
    Renderer::Print(Message);
}
