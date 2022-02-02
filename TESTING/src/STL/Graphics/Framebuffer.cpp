#include "Framebuffer.h"
#include "Graphics.h"
#include "ARGB.h"

#include "STL/Memory/Memory.h"
#include "STL/String/cstr.h"
#include "STL/Math/Math.h"

namespace STL
{		
    void Framebuffer::Clear()
    {
        STL::SetMemory(this->Base, 0, this->Size);
    }

    void Framebuffer::ScrollUp(uint32_t Amount)
    {
        uint64_t Offset = this->PixelsPerScanline * Amount;
        STL::CopyMemory(this->Base + Offset, this->Base, this->Size - Offset * 4);
        STL::SetMemory(this->Base + this->PixelsPerScanline * (this->Height - Amount), 0, Offset * 4);
    }

    ARGB Framebuffer::GetPixel(Point Pixel)
    {
        if (Pixel.X > this->Width || Pixel.X < 0 || Pixel.Y > this->Height || Pixel.Y < 0)
        {
            return ARGB(0);
        }

        return *(ARGB*)((uint64_t)this->Base + Pixel.X * 4 + Pixel.Y * this->PixelsPerScanline * 4);
    }

    void Framebuffer::PutPixel(Point Pixel, ARGB Color)
    {
        if (Pixel.X > this->Width || Pixel.X < 0 || Pixel.Y > this->Height || Pixel.Y < 0)
        {
            return;
        }

        *(ARGB*)((uint64_t)this->Base + Pixel.X * 4 + Pixel.Y * this->PixelsPerScanline * 4) = Color;
    }

    void Framebuffer::DrawRaisedRectEdge(Point TopLeft, Point BottomRight)
    {
        for (int y = TopLeft.Y - RAISEDWIDTH; y < TopLeft.Y; y++)
        {
            for (int x = TopLeft.X - RAISEDWIDTH; x < BottomRight.X + RAISEDWIDTH; x++)
            {                   
                this->PutPixel(STL::Point(x, y), RAISEDHIGHCOLOR);                    
            }
        }

        for (int y = BottomRight.Y; y < BottomRight.Y + RAISEDWIDTH; y++)
        {
            for (int x = TopLeft.X - RAISEDWIDTH; x < BottomRight.X + RAISEDWIDTH; x++)
            {                   
                this->PutPixel(STL::Point(x, y), RAISEDLOWCOLOR);                    
            }
        }

        for (int y = TopLeft.Y - RAISEDWIDTH; y < BottomRight.Y + RAISEDWIDTH; y++)
        {
            for (int x = TopLeft.X - RAISEDWIDTH; x < TopLeft.X; x++)
            {                   
                this->PutPixel(STL::Point(x, y), RAISEDHIGHCOLOR);                    
            }
        }

        for (int y = TopLeft.Y - RAISEDWIDTH; y < BottomRight.Y + RAISEDWIDTH; y++)
        {
            for (int x = BottomRight.X; x < BottomRight.X + RAISEDWIDTH; x++)
            {                   
                this->PutPixel(STL::Point(x, y), RAISEDLOWCOLOR);                    
            }
        }

        for (int y = 0; y < RAISEDWIDTH; y++)
        {
            for (int x = 0; x < RAISEDWIDTH; x++)
            {
                if (y + x < RAISEDWIDTH)
                {
                    this->PutPixel(STL::Point(BottomRight.X + x, TopLeft.Y - RAISEDWIDTH + y), RAISEDHIGHCOLOR);
                    this->PutPixel(STL::Point(TopLeft.X - RAISEDWIDTH + x, BottomRight.Y + y), RAISEDHIGHCOLOR);                    
                }
                else
                {
                    this->PutPixel(STL::Point(BottomRight.X + x, TopLeft.Y - RAISEDWIDTH + y), RAISEDLOWCOLOR);
                    this->PutPixel(STL::Point(TopLeft.X - RAISEDWIDTH + x, BottomRight.Y + y), RAISEDLOWCOLOR);
                }
            }
        }          
    }

    void Framebuffer::DrawRaisedRect(Point TopLeft, Point BottomRight, ARGB Color)
    {
        DrawRect(TopLeft, BottomRight, Color);

        DrawRaisedRectEdge(TopLeft, BottomRight);
    }

    void Framebuffer::DrawSunkenRectEdge(Point TopLeft, Point BottomRight)
    {
        for (int y = TopLeft.Y - RAISEDWIDTH; y < TopLeft.Y; y++)
        {
            for (int x = TopLeft.X - RAISEDWIDTH; x < BottomRight.X + RAISEDWIDTH; x++)
            {                   
                this->PutPixel(STL::Point(x, y), RAISEDLOWCOLOR);                    
            }
        }

        for (int y = BottomRight.Y; y < BottomRight.Y + RAISEDWIDTH; y++)
        {
            for (int x = TopLeft.X - RAISEDWIDTH; x < BottomRight.X + RAISEDWIDTH; x++)
            {                   
                this->PutPixel(STL::Point(x, y), RAISEDHIGHCOLOR);                    
            }
        }

        for (int y = TopLeft.Y - RAISEDWIDTH; y < BottomRight.Y + RAISEDWIDTH; y++)
        {
            for (int x = TopLeft.X - RAISEDWIDTH; x < TopLeft.X; x++)
            {                   
                this->PutPixel(STL::Point(x, y), RAISEDLOWCOLOR);                    
            }
        }

        for (int y = TopLeft.Y - RAISEDWIDTH; y < BottomRight.Y + RAISEDWIDTH; y++)
        {
            for (int x = BottomRight.X; x < BottomRight.X + RAISEDWIDTH; x++)
            {                   
                this->PutPixel(STL::Point(x, y), RAISEDHIGHCOLOR);                    
            }
        }

        for (int y = 0; y < RAISEDWIDTH; y++)
        {
            for (int x = 0; x < RAISEDWIDTH; x++)
            {
                if (y + x < RAISEDWIDTH)
                {
                    this->PutPixel(STL::Point(BottomRight.X + x, TopLeft.Y - RAISEDWIDTH + y), RAISEDLOWCOLOR);
                    this->PutPixel(STL::Point(TopLeft.X - RAISEDWIDTH + x, BottomRight.Y + y), RAISEDLOWCOLOR);                    
                }
                else
                {
                    this->PutPixel(STL::Point(BottomRight.X + x, TopLeft.Y - RAISEDWIDTH + y), RAISEDHIGHCOLOR);
                    this->PutPixel(STL::Point(TopLeft.X - RAISEDWIDTH + x, BottomRight.Y + y), RAISEDHIGHCOLOR);
                }
            }
        }
    }

    void Framebuffer::DrawSunkenRect(Point TopLeft, Point BottomRight, ARGB Color)
    {
        DrawRect(TopLeft, BottomRight, Color);

        DrawSunkenRectEdge(TopLeft, BottomRight);
    }

    void Framebuffer::DrawRect(STL::Point TopLeft, STL::Point BottomRight, ARGB Color)
    {        
        if (TopLeft.X > this->Width || TopLeft.X < 0 || TopLeft.Y > this->Height || TopLeft.Y < 0)
        {
            return;
        }

        if (BottomRight.X > this->Width || BottomRight.X < 0 || BottomRight.Y > this->Height || BottomRight.Y < 0)
        {
            return;
        }

        for (int y = TopLeft.Y; y < BottomRight.Y; y++)
        {
            for (int x = TopLeft.X; x < BottomRight.X; x++)
            {
                *(ARGB*)((uint64_t)this->Base + x * 4 + y * this->PixelsPerScanline * 4) = Color;
            }
        }
    }

    void Framebuffer::Fill(ARGB Color)
    {
        for (int y = 0; y < this->Height; y++)
        {
            for (int x = 0; x < this->Width; x++)
            {
                *(ARGB*)((uint64_t)this->Base + x * 4 + y * this->PixelsPerScanline * 4) = Color;
            }
        }        
    }

    void Framebuffer::PutChar(char chr, STL::Point Pos, uint8_t Scale, ARGB Foreground, ARGB Background)
    {
        char* Glyph = GetFont()->glyphBuffer + (chr * GetFont()->PSF_header->charsize);

        for (uint64_t y = 0; y < 16 * Scale; y++)
        {
            for (uint64_t x = 0; x < 8 * Scale; x++)
            {
                if ((*Glyph & (0b10000000 >> x / Scale)) > 0)
                {
                    this->PutPixel(Point(Pos.X + x, Pos.Y + y), Foreground);
                }
                else
                {
                    this->PutPixel(Point(Pos.X + x, Pos.Y + y), Background);
                }
            }
            if (y % Scale == 0)
            {
                Glyph++;
            }
        }
    }

    void Framebuffer::Print(const char* cstr, STL::Point& Pos, uint8_t Scale, ARGB Foreground, ARGB Background, uint32_t NewLineOffset)
    {
        for (int i = 0; i < Length(cstr); i++)
        {
            switch (cstr[i])
            {
            case '\n':
            {
                Pos.Y += 16 * Scale;
            }
            break;
            case '\r':
            {
                Pos.X = NewLineOffset;
            }
            break;
            case '\033':
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
            break;
            default:
            {
                if (Pos.X + NewLineOffset + 16 * Scale > this->Width)
                {
                    Pos.X = NewLineOffset;
                    Pos.Y += 16 * Scale;
                }

                PutChar(cstr[i], Pos, Scale, Foreground, Background);
                Pos.X += 8 * Scale;
            }
            break;
            }  
        }
    }
}