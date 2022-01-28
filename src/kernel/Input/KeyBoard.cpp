#include "KeyBoard.h"
#include "Mouse.h"
#include "kernel/Renderer/Renderer.h"

namespace KeyBoard
{
    /// <summary>
    /// A table to convert from keyboard scancodes to ascii.
    /// The index in the table is the scancode value and the given element the ascii value or if an ascii.
    /// </summary>
    const char ScanCodeTable[] =
    {
        0,  0, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
        '9', '0', '-', '=', BACKSPACE,	/* Backspace */
        '\t',			/* Tab */
        'q', 'w', 'e', 'r',	/* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', ENTER,	/* Enter key */
        CONTROL,			/* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
        '\'', '`', LEFT_SHIFT,		/* Left shift */
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

    /// <summary>
    /// A table of what keys are pressed.
    /// </summary>
    volatile bool PressedTable[255];

    /// <summary>
    /// The last pressed key.
    /// </summary>
    volatile uint8_t CurrentKey = 0;

    void Clear()
    {
        for (int i = 0; i < 255; i++)
        {
            PressedTable[i] = false;
        }
    }

    void HandleScanCode(uint8_t ScanCode)
    {
        bool IsUp = ScanCode & (0b10000000);
        ScanCode &= ~(0b10000000);

        uint8_t Key = ScanCodeTable[ScanCode];

        if (IsHeld(LEFT_SHIFT) && Key != LEFT_SHIFT)
        {
            Key &= ~(0b00100000);
        }
        else if (Key == LEFT_SHIFT)
        {
            PressedTable[Key] = !IsUp;
            return;
        }

        switch (Key)
        {
        case ARROW_UP:
        {
            Mouse::Position.Y -= 10;
            Mouse::Position.Y = STL::Clamp(Mouse::Position.Y, 0, Renderer::GetScreenSize().Y - 16);
        }
        break;
        case ARROW_DOWN:
        {
            Mouse::Position.Y += 10;
            Mouse::Position.Y = STL::Clamp(Mouse::Position.Y, 0, Renderer::GetScreenSize().Y - 16);
        }
        break;
        case ARROW_LEFT:
        {
            Mouse::Position.X -= 10;
            Mouse::Position.X = STL::Clamp(Mouse::Position.X, 0, Renderer::GetScreenSize().X - 8);
        }
        break;
        case ARROW_RIGHT:
        {
            Mouse::Position.X += 10;
            Mouse::Position.X = STL::Clamp(Mouse::Position.X, 0, Renderer::GetScreenSize().X - 8);
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

    char GetKeyHeld()
    {
        return CurrentKey;
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

    bool IsHeld(char Key)
    {
        return PressedTable[Key];
    }
}
