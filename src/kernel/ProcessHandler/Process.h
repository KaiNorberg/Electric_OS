#pragma once

#include "STL/Process/Process.h"
#include "STL/Graphics/Framebuffer.h"
#include "STL/String/String.h"

#include "kernel/Renderer/Renderer.h"

#define FRAME_OFFSET STL::Point(0, 30)
#define CLOSE_BUTTON_SIZE STL::Point(16, 16)
#define CLOSE_BUTTON_OFFSET STL::Point(((FRAME_OFFSET.X - CLOSE_BUTTON_SIZE.X) / 2 - CLOSE_BUTTON_SIZE.X), -FRAME_OFFSET.Y / 2 - CLOSE_BUTTON_SIZE.Y / 2)
#define MOVING_WINDOW_OUTLINE_AMOUNT 100 * 2
#define MOVING_WINDOW_OUTLINE_SPEED 10
#define MOVING_WINDOW_OUTLINE_THICKNESS RAISEDWIDTH
#define MOVING_WINDOW_OUTLINE_COLOR STL::ARGB(255, 224, 108, 117)

class Process
{
public:

    uint64_t GetID();

    STL::Point GetPos();

    void SetPos(STL::Point NewPos);

    STL::Point GetSize();

    STL::PROT GetType();

    STL::PROR GetRequest();

    STL::PROC GetProcedure();

    STL::Point GetCloseButtonPos();

    void SendRequest(STL::PROR Request);

    void SetDepth(uint64_t Depth);

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

    STL::String Title;

    uint64_t RequestAmount;
    STL::PROR Requests[16];
};