#include "Graphics.h"

#include "STL/String/cstr.h"

namespace STL
{
    PSF_FONT* CurrentFont = nullptr;

    void SetFont(PSF_FONT* NewFont)
    {
        CurrentFont = NewFont;
    }

    void PutChar(Framebuffer* TargetBuffer, char chr, STL::Point Pos, uint8_t Scale, ARGB Foreground, ARGB Background)
    {
        char* Glyph = CurrentFont->glyphBuffer + (chr * CurrentFont->PSF_header->charsize);

        for (uint64_t y = 0; y < 16 * Scale; y++)
        {
            for (uint64_t x = 0; x < 8 * Scale; x++)
            {
                if ((*Glyph & (0b10000000 >> x / Scale)) > 0)
                {
                    TargetBuffer->PutPixel(Point(Pos.X + x, Pos.Y + y), Foreground);
                }
                else
                {
                    TargetBuffer->PutPixel(Point(Pos.X + x, Pos.Y + y), Background);
                }
            }
            if (y % Scale == 0)
            {
                Glyph++;
            }
        }
    }

    void Print(Framebuffer* TargetBuffer, const char* cstr, STL::Point& Pos, uint8_t Scale, ARGB Foreground, ARGB Background)
    {
        for (int i = 0; i < Length(cstr); i++)
        {
            if (cstr[i] == '\n')
            {
                Pos.Y += 16 * Scale;
            }
            else if (cstr[i] == '\r')
            {
                Pos.X = 0;
            }
            else if (cstr[i] == '\033')
            {
                if (i + 10 > STL::Length(cstr))
                {
                    return;
                }

                i++;
                switch (cstr[i])
                {
                case 'F':
                {
                    Foreground = STL::ARGB(255, (cstr[i + 1] - '0') * 100 + (cstr[i + 2] - '0') * 10 + (cstr[i + 3] - '0'), 
                    (cstr[i + 4] - '0') * 100 + (cstr[i + 5] - '0') * 10 + (cstr[i + 6] - '0'), 
                    (cstr[i + 7] - '0') * 100 + (cstr[i + 8] - '0') * 10 + (cstr[i + 9] - '0'));
                }
                break;
                case 'B':
                {
                    Background = STL::ARGB(255, (cstr[i + 1] - '0') * 100 + (cstr[i + 2] - '0') * 10 + (cstr[i + 3] - '0'), 
                    (cstr[i + 4] - '0') * 100 + (cstr[i + 5] - '0') * 10 + (cstr[i + 6] - '0'), 
                    (cstr[i + 7] - '0') * 100 + (cstr[i + 8] - '0') * 10 + (cstr[i + 9] - '0'));
                }
                break;
                }
                i += 9;
            }
            else
            {        
                if (Pos.X + 16 * Scale > TargetBuffer->Width)
                {
                    Pos.X = 0;
                    Pos.Y += 16 * Scale;
                }

                PutChar(TargetBuffer, cstr[i], Pos, Scale, Foreground, Background);
                Pos.X += 8 * Scale;
            }     
        }
    }
}