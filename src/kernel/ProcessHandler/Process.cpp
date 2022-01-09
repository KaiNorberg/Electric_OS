#include "Process.h"
#include "ProcessHandler.h"

#include "kernel/Memory/Heap.h"

uint64_t Process::GetID()
{
    return this->ID;
}

STL::Point Process::GetPos()
{
    return this->Pos;
}

STL::PROT Process::GetType()
{
    return this->Type;
}

STL::PROR Process::GetRequest()
{
    STL::PROR Temp = this->Request;
    this->Request = STL::PROR::SUCCESS;
    return Temp;
}

void Process::Clear()
{
    if (this->Type == STL::PROT::FULLSCREEN)
    {  
        this->FrameBuffer.Clear();
        this->SendMessage(STL::PROM::CLEAR, &this->FrameBuffer);
    } 
}

void Process::Kill()
{
    this->SendMessage(STL::PROM::KILL, nullptr);

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

    this->SendMessage(STL::PROM::DRAW, &this->FrameBuffer);

    if (this->Type == STL::PROT::FULLSCREEN)
    {   
        STL::CopyMemory(this->FrameBuffer.Base, Renderer::Backbuffer.Base, this->FrameBuffer.Size);
    }

    ProcessHandler::SwapBuffersRequest = true;
}

void Process::SendMessage(STL::PROM Message, STL::PROI Input)
{
    ProcessHandler::LastMessagedProcess = this->ID;

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
        this->FrameBuffer = Renderer::Backbuffer;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(Renderer::Backbuffer.Size);
        this->FrameBuffer.Clear();
        this->Draw();
    }
} 