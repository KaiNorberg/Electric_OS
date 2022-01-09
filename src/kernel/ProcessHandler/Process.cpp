#include "Process.h"
    
#include "kernel/Memory/Heap.h"

void Process::Kill()
{
    this->SendMessage(STL::PROM::EXIT, nullptr);


    if (this->Type == STL::PROT::FULLSCREEN)
    {
        FrameBuffer.Clear();
        Heap::Free(FrameBuffer.Base);
    }
}

void Process::Draw()
{
    if (Type == STL::PROT::BACKGROUND)
    {
        return;
    }

    this->SendMessage(STL::PROM::DRAW, &FrameBuffer);

    if (this->Type == STL::PROT::FULLSCREEN)
    {   
        STL::CopyMemory(FrameBuffer.Base, Renderer::Screenbuffer->Base, FrameBuffer.Size);
        Renderer::RedrawMouse();
    }
}

void Process::SendMessage(STL::PROM Message, STL::PROI Input)
{
    STL::PROR NewRequest = this->Procedure(Message, Input);

    if (this->Request != STL::PROR::SUCCESS && NewRequest == STL::PROR::SUCCESS)
    {
        return;
    }
    this->Request = NewRequest;
}

Process::Process(STL::PROC Procedure)
{
    static uint64_t NewID = 0;
    NewID++;

    this->ID = NewID;
    this->Procedure = Procedure;

    STL::PINFO Info;
    this->SendMessage(STL::PROM::INIT, &Info);

    this->Depth = Info.Depth;
    this->Type = Info.Type;

    this->Pos = STL::Point(Info.Left, Info.Top);

    if (Info.Type == STL::PROT::FULLSCREEN)
    {        
        this->Pos = STL::Point(0, 0);
        this->FrameBuffer = *Renderer::Screenbuffer;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(Renderer::Screenbuffer->Size);
        this->FrameBuffer.Clear();
        this->Draw();
    }
} 