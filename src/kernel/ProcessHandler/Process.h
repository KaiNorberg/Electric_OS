#pragma once

#include "STL/Process/Process.h"
#include "STL/Graphics/Framebuffer.h"

#include "kernel/Renderer/Renderer.h"

struct Process
{
    STL::PROT Type;
    STL::PROC Procedure;
    STL::Framebuffer FrameBuffer;

    uint64_t Depth;

    STL::Point Pos;

    void Draw();

    void SendMessage(STL::PROM Message, STL::PROI Input = nullptr);

    Process(STL::PROC Procedure);
};