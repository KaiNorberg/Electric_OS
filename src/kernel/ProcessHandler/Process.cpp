#include "Process.h"

void Process::Draw()
{
    if (Type == STL::PROT::BACKGROUND)
    {
        return;
    }

    this->SendMessage(STL::PROM::DRAW, &FrameBuffer);

    if (this->Type == STL::PROT::WINDOWED)
    {
        /*for (int y = 0; y < FrameBuffer.Height; y++)
        {
            if (y + Pos.Y > FrameBuffer.Height || X + Pos.X > FrameBuffer.Width)
            {
                return;
            }

            STL::CopyMemory(
                FrameBuffer.Base + y * FrameBuffer.Width * 4,
                Renderer::Screenbuffer->Base + (Pos.Y + y) * Renderer::Screenbuffer->Width * 4,
                FrameBuffer.Width * 4
            );
        }*/
    }
    else if (this->Type == STL::PROT::FULLSCREEN)
    {   
        STL::CopyMemory(FrameBuffer.Base, Renderer::Screenbuffer->Base, FrameBuffer.Size);
        Renderer::RedrawMouse();
    }
}

void Process::SendMessage(STL::PROM Message, STL::PROI Input)
{
    STL::PROR Return = this->Procedure(Message, Input);

    if (Return == STL::PROR::REDRAW)
    {
        RedrawRequested = true;
    }
}

Process::Process(STL::PROC Procedure)
{
    this->Procedure = Procedure;
} 