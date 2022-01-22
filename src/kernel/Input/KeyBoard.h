#pragma once

#include <stdint.h>

#define ENTER 0x1C
#define BACKSPACE 0x0E
#define CONTROL 0x1D
#define LEFT_SHIFT 0x2A
#define ARROW_UP 0x48
#define ARROW_DOWN 0x50
#define ARROW_LEFT 0x4B
#define ARROW_RIGHT 0x4D
#define PAGE_UP 0x49
#define PAGE_DOWN 0x51

namespace KeyBoard
{    
    void Clear();

    void HandleScanCode(uint8_t ScanCode);

    char GetKeyPress();

    char GetKeyHeld();

    bool IsPressed(char Key);

    bool IsHeld(char Key);
}