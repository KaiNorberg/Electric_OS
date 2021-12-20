#include "Panic.h"
#include "../Rendering/Renderer.h"

void Panic(const char* Message)
{
    Renderer::Clear(ARGB(255, 255, 0, 0));
    Renderer::Print("KERNEL PANIC!\n\r");
    Renderer::Print("UNEXPECTED ERROR OCCURRED\n\r");
    Renderer::Print("ERROR RETURNED FOLLOWING MESSAGE: ");
    Renderer::Print(Message);
<<<<<<< HEAD:src/kernel/Core/Panic.cpp
    
    asm("HLT");
=======
>>>>>>> parent of 0d6c9a1 (Started work on system calls.):kernel/src/Core/Panic.cpp
}
