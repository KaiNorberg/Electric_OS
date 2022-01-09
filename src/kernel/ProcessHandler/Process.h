#pragma once

#include "STL/Process/Process.h"
#include "STL/Graphics/Framebuffer.h"

#include "kernel/Renderer/Renderer.h"

class Process
{
public:

    uint64_t GetID();

    STL::Point GetPos();

    STL::PROT GetType();

    STL::PROR GetRequest();

    void Clear();

    void Kill();

    void Draw();

    void SendMessage(STL::PROM Message, STL::PROI Input = nullptr);

    Process(STL::PROC Procedure);

private:

    uint64_t ID;

    STL::PROT Type;
    STL::PROC Procedure;
    STL::Framebuffer FrameBuffer;

    uint64_t Depth;
    STL::Point Pos;

    STL::PROR Request = STL::PROR::SUCCESS; 
};