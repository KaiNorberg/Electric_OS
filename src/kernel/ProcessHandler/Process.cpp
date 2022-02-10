#include "Process.h"
#include "ProcessHandler.h"

#include "STL/Math/Math.h"

#include "kernel/Memory/Heap.h"

uint64_t Process::GetID()
{
    return this->ID;
}

STL::Point Process::GetPos()
{
    return this->Pos;
}

STL::Point Process::GetSize()
{
    return STL::Point(this->FrameBuffer.Width, this->FrameBuffer.Height);
}

void Process::SetPos(STL::Point NewPos)
{
    this->Pos = NewPos;

    if (this->Type == STL::PROT::WINDOWED)
    {
        this->Pos.X = STL::Clamp(this->Pos.X, (int32_t)RAISEDWIDTH, (int32_t)(Renderer::Backbuffer.Width - this->FrameBuffer.Width - RAISEDWIDTH));
        this->Pos.Y = STL::Clamp(this->Pos.Y, (int32_t)RAISEDWIDTH + FRAME_OFFSET.Y, (int32_t)(Renderer::Backbuffer.Height - this->FrameBuffer.Height - RAISEDWIDTH));        
    }
    else
    {
        this->Pos.X = STL::Clamp(this->Pos.X, (int32_t)0, (int32_t)(Renderer::Backbuffer.Width - this->FrameBuffer.Width));
        this->Pos.Y = STL::Clamp(this->Pos.Y, (int32_t)0, (int32_t)(Renderer::Backbuffer.Height - this->FrameBuffer.Height));        
    }
}

STL::PROT Process::GetType()
{
    return this->Type;
}

const char* Process::GetTitle()
{
    return this->Title.cstr();
}

STL::PROR Process::GetRequest()
{
    if (RequestAmount > 0)
    {        
        RequestAmount--;
        return Requests[RequestAmount];
    }   
    return STL::PROR::SUCCESS;
}

void Process::SendRequest(STL::PROR Request)
{
    if (Request != STL::PROR::SUCCESS && RequestAmount < 16)
    {
        Requests[RequestAmount] = Request;
        RequestAmount++;
    }
}

STL::PROC Process::GetProcedure()
{
    return this->Procedure;
}

STL::Point Process::GetCloseButtonPos()
{
    return CLOSE_BUTTON_OFFSET + STL::Point(this->FrameBuffer.Width, 0) + this->Pos; 
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

bool Process::Contains(STL::Point Other)
{
    return (this->Pos.X < Other.X && this->Pos.X + this->FrameBuffer.Width > Other.X &&
            this->Pos.Y < Other.Y && this->Pos.Y + this->FrameBuffer.Height > Other.Y);
}

bool Process::Contains(Process* Other)
{
    return (this->Pos.X <= Other->Pos.X + Other->FrameBuffer.Width && this->Pos.X + this->FrameBuffer.Width >= Other->Pos.X &&
            this->Pos.Y <= Other->Pos.Y + Other->FrameBuffer.Height && this->Pos.Y + this->FrameBuffer.Height >= Other->Pos.Y);
}

void Process::Clear()
{ 
    this->FrameBuffer.Clear();
    this->SendMessage(STL::PROM::CLEAR, &this->FrameBuffer);
}

void Process::Kill()
{
    this->SendMessage(STL::PROM::KILL, nullptr);

    Heap::Free(FrameBuffer.Base);
}

void Process::Draw()
{
    this->SendMessage(STL::PROM::DRAW, &this->FrameBuffer);
}

void Process::Render()
{
    if (this->Type == STL::PROT::WINDOWED)
    {         
        STL::ARGB Background;
        STL::ARGB Foreground;
        if (this == ProcessHandler::FocusedProcess)
        {
            Background = STL::ARGB(255, 14, 0, 135);            
            Foreground = STL::ARGB(255);
        }
        else
        {
            Background = STL::ARGB(128);            
            Foreground = STL::ARGB(192);            
        }

        //Draw topbar
        Renderer::Backbuffer.DrawRaisedRectEdge(this->Pos - FRAME_OFFSET, this->Pos + STL::Point(this->FrameBuffer.Width, this->FrameBuffer.Height));
        Renderer::Backbuffer.DrawRect(this->Pos - FRAME_OFFSET, this->Pos + STL::Point(this->FrameBuffer.Width, 0), Background);

        //Draw close button
        Renderer::Backbuffer.DrawRaisedRect(this->GetCloseButtonPos(), this->GetCloseButtonPos() + CLOSE_BUTTON_SIZE, STL::ARGB(200));

        //Print Title
        STL::Point TextPos = this->Pos + STL::Point(RAISEDWIDTH * 2, -FRAME_OFFSET.Y / 2 - 8);
        Renderer::Backbuffer.Print(this->Title.cstr(), TextPos, 1, Foreground, Background);
    } 

    if (this->Pos.X < 0 || this->Pos.X + this->FrameBuffer.Width > Renderer::Backbuffer.Width || this->Pos.Y < 0 || this->Pos.Y + this->FrameBuffer.Height > Renderer::Backbuffer.Height)
    {
        return;
    }

    //Copy this->FrameBuffer to Renderer::Backbuffer

    uint8_t* Source = (uint8_t*)(this->FrameBuffer.Base);
    uint8_t* Dest = (uint8_t*)(Renderer::Backbuffer.Base + this->Pos.X + Renderer::Backbuffer.PixelsPerScanline * this->Pos.Y);

    for (int y = 0; y < this->FrameBuffer.Height; y++)
    {             
        uint8_t* TempSource = Source;
        uint8_t* TempDest = Dest;
        Source += this->FrameBuffer.PixelsPerScanline * 4;
        Dest += Renderer::Backbuffer.PixelsPerScanline * 4;   

        while (TempSource + 24 < Source)
        {
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"(TempSource), "r"(TempDest) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"(TempSource + 8), "r"(TempDest + 8) : "memory");
            __asm__ __volatile__ ("movq (%0), %%mm0\n" "movq %%mm0, (%1)\n" :: "r"(TempSource + 16), "r"(TempDest + 16) : "memory");

            TempSource += 24;
            TempDest += 24;
        }

        while (TempSource < Source)
        {
            *TempDest = *TempSource;
            TempSource++;
            TempDest++;
        }
    }
}

void Process::SendMessage(STL::PROM Message, STL::PROI Input)
{
    ProcessHandler::LastMessagedProcess = this;

    STL::PROR NewRequest = this->Procedure(Message, Input);

    this->SendRequest(NewRequest);

    ProcessHandler::LastMessagedProcess = nullptr;
}

Process::Process(STL::PROC Procedure)
{
    static uint64_t NewID = 0;
    NewID++;

    this->ID = NewID;
    this->Procedure = Procedure;
    this->RequestAmount = 0;

    STL::PINFO Info;
    this->SendMessage(STL::PROM::INIT, &Info);

    this->Type = Info.Type;

    this->Pos = STL::Point(Info.Left, Info.Top);
    this->Title = Info.Title;
    this->SetDepth(Info.Depth);

    if (Info.Type == STL::PROT::FULLSCREEN)
    {            
        this->Pos = STL::Point(0, 0);
        this->FrameBuffer.Height = Renderer::Backbuffer.Height;  
        this->FrameBuffer.Width = Renderer::Backbuffer.Width;
        this->FrameBuffer.PixelsPerScanline = Renderer::Backbuffer.Width + 1;
        this->FrameBuffer.Size = (this->FrameBuffer.Height + 1) * this->FrameBuffer.PixelsPerScanline * 4;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(this->FrameBuffer.Size);
    
        this->FrameBuffer.Clear();
        this->SendRequest(STL::PROR::DRAW);
    }
    else if (Info.Type == STL::PROT::FRAMELESSWINDOW || Info.Type == STL::PROT::WINDOWED)
    {               
        this->FrameBuffer.Height = Info.Height;  
        this->FrameBuffer.Width = Info.Width;
        this->FrameBuffer.PixelsPerScanline = Info.Width + 1;
        this->FrameBuffer.Size = (this->FrameBuffer.Height + 1) * this->FrameBuffer.PixelsPerScanline * 4;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(this->FrameBuffer.Size);

        this->FrameBuffer.Clear();
        this->SendRequest(STL::PROR::DRAW);
    }
} 