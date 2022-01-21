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

uint64_t Process::GetDepth()
{
    return this->Depth;
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
    if (this->Type == STL::PROT::FULLSCREEN)
    {   
        STL::CopyMemory(this->FrameBuffer.Base, Renderer::Backbuffer.Base, this->FrameBuffer.Size);
    }
    else if (this->Type == STL::PROT::FRAMELESSWINDOW)
    {
        /*if (this->TopLeft.X > this->Width || TopLeft.X < 0 || TopLeft.Y > this->Height || TopLeft.Y < 0)
        {
            return;
        }

        if (BottomRight.X > this->Width || BottomRight.X < 0 || BottomRight.Y > this->Height || BottomRight.Y < 0)
        {
            return;
        }*/

        for (int y = 0; y < this->Size.Y; y++)
        {
            for (int x = 0; x < this->Size.X; x++)
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
    this->Size = STL::Point(Info.Width, Info.Height);

    if (Info.Type == STL::PROT::FULLSCREEN)
    {            
        this->Depth = 0;
        this->Pos = STL::Point(0, 0);
        this->FrameBuffer = Renderer::Backbuffer;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(Renderer::Backbuffer.Size);
        this->FrameBuffer.Clear();
        this->Draw();
    }
    else if (Info.Type == STL::PROT::FRAMELESSWINDOW)
    {        
        this->Depth = 1;
        
        this->FrameBuffer.Height = Info.Height;  
        this->FrameBuffer.Width = Info.Width;
        this->FrameBuffer.PixelsPerScanline = Info.Width + 1;
        this->FrameBuffer.Size = (this->FrameBuffer.Height + 1) * this->FrameBuffer.PixelsPerScanline * 4;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(this->FrameBuffer.Size);

        this->FrameBuffer.Clear();
        this->Draw();
    }
} 