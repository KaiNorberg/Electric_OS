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
    
void Process::SetDepth(uint64_t Depth)
{
    if (Depth > ProcessHandler::Processes.Length())
    {
        return;
    }

    for (int i = 0; i < ProcessHandler::Processes.Length(); i++)
    {
        if (ProcessHandler::Processes[i] == this)
        {
            ProcessHandler::Processes[i] = ProcessHandler::Processes[Depth];
            ProcessHandler::Processes[Depth] = this;
            return;
        }
    }
}

void Process::Clear()
{
    if (this->Type == STL::PROT::FULLSCREEN || this->Type == STL::PROT::FRAMELESSWINDOW)
    {  
        this->FrameBuffer.Clear();
        this->SendMessage(STL::PROM::CLEAR, &this->FrameBuffer);
    } 
}

void Process::Kill()
{
    this->SendMessage(STL::PROM::KILL, nullptr);

    if (this->Type == STL::PROT::FULLSCREEN || this->Type == STL::PROT::FRAMELESSWINDOW)
    {
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

    this->Render();
}

void Process::Render()
{
    if (this->Type == STL::PROT::FRAMELESSWINDOW || this->Type == STL::PROT::FULLSCREEN)
    {
        if (this->Pos.X + this->FrameBuffer.Width > Renderer::Backbuffer.Width || this->Pos.X < 0)
        {
            return;
        }
        if (this->Pos.Y + this->FrameBuffer.Height > Renderer::Backbuffer.Height || this->Pos.Y < 0)
        {
            return;
        }

        for (int y = 0; y < this->FrameBuffer.Height; y++)
        {
            for (int x = 0; x < this->FrameBuffer.Width; x++)
            {
                *(STL::ARGB*)((uint64_t)Renderer::Backbuffer.Base + (this->Pos.X + x) * 4 + (this->Pos.Y + y) * Renderer::Backbuffer.PixelsPerScanline * 4) = 
                *(STL::ARGB*)((uint64_t)this->FrameBuffer.Base + x * 4 + y * this->FrameBuffer.PixelsPerScanline * 4);
            }
        }
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

    this->Type = Info.Type;

    this->Pos = STL::Point(Info.Left, Info.Top);

    if (Info.Type == STL::PROT::FULLSCREEN)
    {            
        this->SetDepth(Info.Depth);
        this->Pos = STL::Point(0, 0);
        this->FrameBuffer.Height = Renderer::Backbuffer.Height;  
        this->FrameBuffer.Width = Renderer::Backbuffer.Width;
        this->FrameBuffer.PixelsPerScanline = Renderer::Backbuffer.Width + 1;
        this->FrameBuffer.Size = (this->FrameBuffer.Height + 1) * this->FrameBuffer.PixelsPerScanline * 4;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(this->FrameBuffer.Size);

        this->FrameBuffer.Clear();
        this->Request = STL::PROR::DRAW;
    }
    else if (Info.Type == STL::PROT::FRAMELESSWINDOW)
    {           
        this->SetDepth(Info.Depth);         
        this->FrameBuffer.Height = Info.Height;  
        this->FrameBuffer.Width = Info.Width;
        this->FrameBuffer.PixelsPerScanline = Info.Width + 1;
        this->FrameBuffer.Size = (this->FrameBuffer.Height + 1) * this->FrameBuffer.PixelsPerScanline * 4;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(this->FrameBuffer.Size);

        this->FrameBuffer.Clear();
        this->Request = STL::PROR::DRAW;
    }
} 