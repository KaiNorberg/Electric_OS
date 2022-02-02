#pragma once

#include <stdint.h>

#include "STL/Math/Point.h"
#include "STL/String/String.h"

namespace STL
{
    enum class PROR : uint32_t //Process request
    {
        SUCCESS,
        RESET,
        RENDER,
        CLEAR,
        DRAW,
        KILL
    };

    enum class PROM : uint32_t //Process Message
    {
        INIT,
        CLEAR,
        DRAW,
        KILL,
        TICK,
        MOUSE,
        KEYPRESS
    };

    enum class PROT : uint32_t //Process Type
    {
        FULLSCREEN,
        WINDOWED,
        FRAMELESSWINDOW
    };

    typedef void* PROI; //Process input

    typedef PROR(*PROC)(PROM, PROI); //Procedure

    struct PINFO //Process Info
    {
        PROT Type;

        uint32_t Depth;
        
        uint32_t Width;
        
        uint32_t Height;
        
        uint32_t Left;

        uint32_t Top;

        String Title;
    };

    struct MINFO //Mouse Info
    {       
        Point Pos;

        bool LeftHeld;
        
        bool MiddleHeld;
        
        bool RightHeld;
    };
}   