#include "KeyBoard.h"
#include "Mouse.h"
#include "../Rendering/Renderer.h"

namespace KeyBoard
{
    const char ScanCodeTable[] =
    {
        0,  0, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
        '9', '0', '-', '=', BACKSPACE,	/* Backspace */
        '\t',			/* Tab */
        'q', 'w', 'e', 'r',	/* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER,	/* Enter key */
        CONTROL,			/* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
        '\'', '`', 0,		/* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
        'm', ',', '.', '/',   0,				/* Right shift */
        '*',
        0,	/* Alt */
        ' ',	/* Space bar */
        0,	/* Caps lock */
        0,	/* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,	/* < ... F10 */
        0,	/* 69 - Num lock*/
        0,	/* Scroll Lock */
        0,	/* Home key */
        ARROW_UP,	/* Up Arrow */
        PAGE_UP,	/* Page Up */
        '-',
        ARROW_LEFT,	/* Left Arrow */
        0,
        ARROW_RIGHT,	/* Right Arrow */
        '+',
        0,	/* 79 - End key*/
        ARROW_DOWN,	/* Down Arrow */
        PAGE_DOWN,	/* Page Down */
        0,	/* Insert Key */
        0,	/* Delete Key */
        0,   0,   0,
        0,	/* F11 Key */
        0,	/* F12 Key */
        0,	/* All other keys are undefined */
    };

    volatile bool PressedTable[255];

    volatile uint8_t CurrentKey = 0;

    void HandleScanCode(uint8_t ScanCode)
    {
        bool IsUp = ScanCode & (1 << 7);

        ScanCode &= ~(1UL << 7);

        uint8_t Key = ScanCodeTable[ScanCode];

        switch (Key)
        {
        case ARROW_UP:
        {
            Mouse::YPos -= 10;
            uint64_t YSize;
            Renderer::GetScreenSize(nullptr, &YSize);
            Mouse::YPos = Math::Clamp(Mouse::YPos, 0, YSize - 16);
        }
        break;
        case ARROW_DOWN:
        {
            Mouse::YPos += 10;
            uint64_t YSize;
            Renderer::GetScreenSize(nullptr, &YSize);
            Mouse::YPos = Math::Clamp(Mouse::YPos, 0, YSize - 16);
        }
        break;
        case ARROW_LEFT:
        {
            Mouse::XPos -= 10;
            uint64_t XSize;
            Renderer::GetScreenSize(&XSize, nullptr);
            Mouse::XPos = Math::Clamp(Mouse::XPos, 0, XSize - 8);
        }
        break;
        case ARROW_RIGHT:
        {
            Mouse::XPos += 10;
            uint64_t XSize;
            Renderer::GetScreenSize(&XSize, nullptr);
            Mouse::XPos = Math::Clamp(Mouse::XPos, 0, XSize - 8);
        }
        break;
        default:
        {
            PressedTable[Key] = !IsUp;

            CurrentKey = Key * !IsUp;
        }
        break;
        }
    }

    char GetKeyPress()
    {
        char Key = CurrentKey;
        CurrentKey = 0;
        return Key;
    }

    bool IsPressed(char Key)
    {
        bool IsPressed = PressedTable[Key];
        PressedTable[Key] = false;
        if (CurrentKey == Key)
        {
            CurrentKey = 0;
        }
        return IsPressed;
    }
}
