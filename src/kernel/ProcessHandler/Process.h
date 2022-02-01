#pragma once

#include "STL/Process/Process.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/String/String.h"

#include "kernel/Renderer/Renderer.h"


#define FRAME_OFFSET STL::Point(0, 30)
#define CLOSE_BUTTON_SIZE STL::Point(16, 16)
#define CLOSE_BUTTON_OFFSET STL::Point(((FRAME_OFFSET.X - CLOSE_BUTTON_SIZE.X) / 2 - CLOSE_BUTTON_SIZE.X), -FRAME_OFFSET.Y / 2 - CLOSE_BUTTON_SIZE.Y / 2)

class Process
{
public:

    uint64_t GetID();

    STL::Point GetPos();

    void SetPos(STL::Point NewPos);

    STL::Point GetSize();

    STL::PROT GetType();

    STL::PROR GetRequest();

    void SendRequest(STL::PROR Request);

    void SetDepth(uint64_t Depth);

    bool Contains(STL::Point Other);

    bool Contains(Process* Other);

    bool Overlap(Process* Other);

    void Clear();

    void Kill();

    void Draw();

    void Render(STL::Point TopLeft, STL::Point BottomRight);

    void Render();

    void SendMessage(STL::PROM Message, STL::PROI Input = nullptr);
    
    Process(STL::PROC Procedure);

private:

    uint64_t ID;

    STL::PROT Type;
    STL::PROC Procedure;
    STL::Framebuffer FrameBuffer;

    STL::Point Pos;

    STL::String Title;

    STL::PROR Request = STL::PROR::SUCCESS; 
};