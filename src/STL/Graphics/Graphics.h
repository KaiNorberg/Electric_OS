#pragma once

#include "Font.h"
#include "Framebuffer.h"

namespace STL
{
    void SetFont(PSF_FONT* NewFont);

    void PutChar(Framebuffer* TargetBuffer, char chr, STL::Point Pos, uint8_t Scale, ARGB Foreground, ARGB Background);

    void Print(Framebuffer* TargetBuffer, const char* cstr, STL::Point& Pos, uint8_t Scale, ARGB Foreground, ARGB Background);
}