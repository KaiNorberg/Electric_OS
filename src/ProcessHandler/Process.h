#pragma once

#include "STL/Process/Process.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/String/String.h"

#include "Renderer/Renderer.h"

#define FRAME_OFFSET STL::Point(0, 30)
#define CLOSE_BUTTON_SIZE STL::Point(16, 16)
#define CLOSE_BUTTON_OFFSET STL::Point(((FRAME_OFFSET.X - CLOSE_BUTTON_SIZE.X) / 2 - CLOSE_BUTTON_SIZE.X), -FRAME_OFFSET.Y / 2 - CLOSE_BUTTON_SIZE.Y / 2)
#define MINIMIZE_BUTTON_SIZE STL::Point(16, 16)
#define MINIMIZE_BUTTON_OFFSET CLOSE_BUTTON_OFFSET - STL::Point(16 + RAISED_WIDTH * 3, 0)
#define MINIMIZED_WINDOW_WIDTH 200

class Process
{
public:

    uint64_t GetID();

    STL::Point GetPos();

    void SetPos(STL::Point NewPos);

    STL::Point GetSize();

    STL::PROT GetType();

    const char* GetTitle();

    STL::PROR PopRequest();

    STL::PROC GetProcedure();

    STL::Point GetCloseButtonPos();

    STL::Point GetMinimizeButtonPos();

    void Minimize();

    void PushRequest(STL::PROR Request);

    void SetDepth(uint64_t Depth);

    void UpdateDepth();

    bool Contains(STL::Point Other);

    bool Contains(Process* Other);

    void Clear();

    void Kill();

    void Draw();

    void Render();

    void SendMessage(STL::PROM Message, STL::PROI Input = nullptr);
    
    Process(STL::PROC Procedure);

private:

    uint64_t ID;

    STL::PROT Type;
    STL::PROC Procedure;
    STL::Framebuffer FrameBuffer;

    STL::Point Pos;
    STL::Point OldPos;
    uint64_t Depth;

    STL::String Title;

    uint64_t RequestAmount;
    STL::PROR Requests[16];
};