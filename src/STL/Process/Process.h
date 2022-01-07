#pragma once

#include <stdint.h>

namespace STL
{
    enum PROR //Process return
    {
        FAILURE,
        SUCCESS,
        REDRAW
    };

    enum PROM //Process Message
    {
        INIT,
        DRAW,
        TICK,
        MOUSE,
        KEYPRESS
        //EXIT
    };

    enum PROT //Process Type
    {
        BACKGROUND,
        FULLSCREEN,
        WINDOWED
    };

    typedef void* PROI; //Process input

    typedef PROR(*PROC)(PROM, PROI); //Procedure

    struct PINFO //Process Info
    {
        PROT Type;

        uint64_t Depth;
        
        uint64_t Width;
        
        uint64_t Height;
        
        uint64_t Left;

        uint64_t Top;
    };
}   