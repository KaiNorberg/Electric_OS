#include "Process.h"
#include "ProcessHandler.h"

#include "Debug/Debug.h"

#include "STL/Math/Math.h"

#include "Memory/Heap.h"

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
    switch (this->Type)
    {
    case STL::PROT::MINIMIZED:
    {
        return STL::Point(MINIMIZED_WINDOW_WIDTH, 0);
    }
    break;
    default:
    {
        return STL::Point(this->FrameBuffer.Width, this->FrameBuffer.Height);
    }
    break;
    }
}

void Process::SetPos(STL::Point NewPos)
{
    this->Pos = NewPos;

    switch (this->Type)
    {
    case STL::PROT::WINDOWED:
    {
        STL::Point ScreenSize = Renderer::GetScreenSize();
        this->Pos.X = STL::Clamp(this->Pos.X, (int32_t)RAISED_WIDTH, (int32_t)(ScreenSize.X - this->FrameBuffer.Width - RAISED_WIDTH));
        this->Pos.Y = STL::Clamp(this->Pos.Y, (int32_t)RAISED_WIDTH + FRAME_OFFSET.Y, (int32_t)(ScreenSize.Y - this->FrameBuffer.Height - RAISED_WIDTH));        
    }
    break;    
    case STL::PROT::MINIMIZED:
    {        
        STL::Point ScreenSize = Renderer::GetScreenSize();
        this->Pos.X = STL::Clamp(this->Pos.X, (int32_t)RAISED_WIDTH, (int32_t)(ScreenSize.X - RAISED_WIDTH));
        this->Pos.Y = STL::Clamp(this->Pos.Y, (int32_t)RAISED_WIDTH + FRAME_OFFSET.Y, (int32_t)(ScreenSize.Y - RAISED_WIDTH));        
    }
    break;
    default:
    {        
        STL::Point ScreenSize = Renderer::GetScreenSize();
        this->Pos.X = STL::Clamp(this->Pos.X, (int32_t)0, (int32_t)(ScreenSize.X - this->FrameBuffer.Width));
        this->Pos.Y = STL::Clamp(this->Pos.Y, (int32_t)0, (int32_t)(ScreenSize.Y - this->FrameBuffer.Height));        
    }
    break;
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

STL::PROR Process::PopRequest()
{
    if (this->Type != STL::PROT::MINIMIZED && RequestAmount > 0)
    {        
        RequestAmount--;
        return Requests[RequestAmount];
    }   
    return STL::PROR::SUCCESS;
}

void Process::PushRequest(STL::PROR Request)
{
    if (this->Type != STL::PROT::MINIMIZED && Request != STL::PROR::SUCCESS && RequestAmount < 16)
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
    return CLOSE_BUTTON_OFFSET + STL::Point(this->GetSize().X, 0) + this->Pos; 
}

STL::Point Process::GetMinimizeButtonPos()
{
    return MINIMIZE_BUTTON_OFFSET + STL::Point(this->GetSize().X, 0) + this->Pos; 
}
    
void Process::Minimize()
{
    if (this->Type == STL::PROT::MINIMIZED)
    {
        this->Type = STL::PROT::WINDOWED;
    }
    else if (this->Type == STL::PROT::WINDOWED)
    {
        this->Type = STL::PROT::MINIMIZED;
    }
    this->SetPos(this->Pos);
}

void Process::SetDepth(uint64_t Depth)
{
    if (Depth > ProcessHandler::Processes.Length())
    {
        return;
    }

    this->Depth = Depth;

    for (uint32_t i = 0; i < ProcessHandler::Processes.Length(); i++)
    {
        if (ProcessHandler::Processes[i] == this)
        {
            ProcessHandler::Processes[i] = ProcessHandler::Processes[Depth];
            ProcessHandler::Processes[Depth] = this;
            return;
        }
    }
}

void Process::UpdateDepth()
{
    this->SetDepth(this->Depth);
}

bool Process::Contains(STL::Point Other)
{
    STL::Point Size = this->GetSize();

    return (this->Pos.X < Other.X && this->Pos.X + (int32_t)Size.X > Other.X &&
            this->Pos.Y < Other.Y && this->Pos.Y + (int32_t)Size.Y > Other.Y);
}

bool Process::Contains(Process* Other)
{    
    STL::Point ThisSize = this->GetSize();
    STL::Point OtherSize = Other->GetSize();

    return (this->Pos.X <= Other->Pos.X + (int32_t)OtherSize.X && this->Pos.X + (int32_t)ThisSize.X >= Other->Pos.X &&
            this->Pos.Y <= Other->Pos.Y + (int32_t)OtherSize.Y && this->Pos.Y + (int32_t)ThisSize.Y >= Other->Pos.Y);
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

void Process::Render(STL::Framebuffer* Buffer)
{
    switch (this->Type)
    {    
    case STL::PROT::MINIMIZED:
    case STL::PROT::WINDOWED:
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

        STL::Point Size = this->GetSize();

        //Draw topbar
        Buffer->DrawRaisedRectEdge(this->Pos - FRAME_OFFSET, this->Pos + Size);
        Buffer->DrawRect(this->Pos - FRAME_OFFSET, this->Pos + STL::Point(Size.X, 0), Background);

        //Draw close button                    
        STL::Point CloseButtonPos = this->GetCloseButtonPos();                    
        Buffer->DrawRect(CloseButtonPos, CloseButtonPos + CLOSE_BUTTON_SIZE, STL::ARGB(200));
        Buffer->PutChar('X', CloseButtonPos + STL::Point(RAISED_WIDTH, 2), 1, STL::ARGB(0), STL::ARGB(200));
        Buffer->DrawRaisedRectEdge(CloseButtonPos, CloseButtonPos + CLOSE_BUTTON_SIZE);

        //Draw minimize button                    
        STL::Point MinimizeButtonPos = this->GetMinimizeButtonPos();
        Buffer->DrawRect(MinimizeButtonPos, MinimizeButtonPos + MINIMIZE_BUTTON_SIZE, STL::ARGB(200));
        Buffer->PutChar('_', MinimizeButtonPos + STL::Point(RAISED_WIDTH, 0), 1, STL::ARGB(0), STL::ARGB(200));
        Buffer->DrawRaisedRectEdge(MinimizeButtonPos, MinimizeButtonPos + MINIMIZE_BUTTON_SIZE);

        //Print Title
        STL::Point TextPos = this->Pos + STL::Point(RAISED_WIDTH * 2, -FRAME_OFFSET.Y / 2 - 8);
        Buffer->Print(this->Title.cstr(), TextPos, 1, Foreground, Background); 

        if (this->Type == STL::PROT::MINIMIZED)
        {
            return;
        }   
    }
    //break; //Fall trough
    default:
    {
        if (this->Pos.X < 0 || this->Pos.X + this->FrameBuffer.Width > Buffer->Width || this->Pos.Y < 0 || this->Pos.Y + this->FrameBuffer.Height > Buffer->Height)
        {
            return;
        }


        void* Source = (uint8_t*)(this->FrameBuffer.Base);
        void* Dest = (uint8_t*)(Buffer->Base + this->Pos.X + Buffer->PixelsPerScanline * this->Pos.Y);
        for (uint32_t y = 0; y < this->FrameBuffer.Height; y++)
        {             
            STL::CopyMemory(Source, Dest, this->FrameBuffer.PixelsPerScanline * 4);
            Source = (void*)((uint64_t)Source + this->FrameBuffer.PixelsPerScanline * 4);
            Dest = (void*)((uint64_t)Dest + Buffer->PixelsPerScanline * 4);   
        }  
    }
    break;
    }
}

void Process::SendMessage(STL::PROM Message, STL::PROI Input)
{
    ProcessHandler::LastMessagedProcess = this;

    STL::PROR NewRequest = this->Procedure(Message, Input);

    this->PushRequest(NewRequest);

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

    switch (this->Type)
    {
    case STL::PROT::FULLSCREEN:
    {
        this->Pos = STL::Point(0, 0);
        this->FrameBuffer.Height = Renderer::GetScreenSize().Y;  
        this->FrameBuffer.Width = Renderer::GetScreenSize().X;
        this->FrameBuffer.PixelsPerScanline = Renderer::GetScreenSize().X + 1;
        this->FrameBuffer.Size = (this->FrameBuffer.Height + 1) * this->FrameBuffer.PixelsPerScanline * 4;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(this->FrameBuffer.Size);
    
        this->FrameBuffer.Clear();
        this->PushRequest(STL::PROR::DRAW);
    }
    break;
    case STL::PROT::FRAMELESSWINDOW:
    case STL::PROT::WINDOWED:
    {
        this->FrameBuffer.Height = Info.Height;  
        this->FrameBuffer.Width = Info.Width;
        this->FrameBuffer.PixelsPerScanline = Info.Width + 1;
        this->FrameBuffer.Size = (this->FrameBuffer.Height + 1) * this->FrameBuffer.PixelsPerScanline * 4;
        this->FrameBuffer.Base = (STL::ARGB*)Heap::Allocate(this->FrameBuffer.Size);

        this->FrameBuffer.Clear();
        this->PushRequest(STL::PROR::DRAW);
    }
    break;
    default:
    {
        Debug::Error("Program of unknown type started");
    }
    break;
    }
} 