#include "Tetris.h"

#include "STL/Graphics/Framebuffer.h"
#include "STL/String/String.h"
#include "STL/String/cstr.h"
#include "STL/System/System.h"
#include "STL/GUI/Button.h"
#include "STL/GUI/Label.h"

#define BLOCK_SIZE 30
#define GRID_WIDTH 10
#define GRID_HEIGHT 20

#define GRID_OUTLINE_COLOR STL::ARGB(200)
#define GRID_OUTLINE_SHADOW STL::ARGB(255)
#define GRID_OUTLINE_HIGHTLIGHT STL::ARGB(60)

namespace Tetris
{
    enum class BlockType
    {
        GRID_OUTLINE
    };

    STL::Point GetGridPosition(STL::Point Pos)
    {
        return (Pos + 1) * BLOCK_SIZE;
    }

    void DrawBlock(STL::Framebuffer* Buffer, STL::Point GridPos, STL::ARGB Color, STL::ARGB Shadow, STL::ARGB HighLight)
    {
        GridPos = GetGridPosition(GridPos);

        Buffer->DrawRectEdge(GridPos + RAISED_WIDTH, GridPos + BLOCK_SIZE - RAISED_WIDTH, HighLight, Shadow);
        Buffer->DrawRect(GridPos + RAISED_WIDTH, GridPos + BLOCK_SIZE - RAISED_WIDTH, Color);
    }

    void DrawBlock(STL::Framebuffer* Buffer, STL::Point GridPos, BlockType Type)
    {
        switch (Type)
        {
        case BlockType::GRID_OUTLINE:
        {
            DrawBlock(Buffer, GridPos, GRID_OUTLINE_COLOR, GRID_OUTLINE_HIGHTLIGHT, GRID_OUTLINE_SHADOW);
        }   
        break;         
        }
    }

    void DrawGridOutline(STL::Framebuffer* Buffer)
    {
        for (int i = -1; i < GRID_HEIGHT + 1; i++)
        {
            DrawBlock(Buffer, STL::Point(-1, i), BlockType::GRID_OUTLINE);
            DrawBlock(Buffer, STL::Point(GRID_WIDTH, i), BlockType::GRID_OUTLINE);
        }        
        
        for (int i = -1; i < GRID_WIDTH + 1; i++)
        {
            DrawBlock(Buffer, STL::Point(i, -1), BlockType::GRID_OUTLINE);
            DrawBlock(Buffer, STL::Point(i, GRID_HEIGHT), BlockType::GRID_OUTLINE);
        }
    }

    STL::PROR Procedure(STL::PROM Message, STL::PROI Input)
    {
        switch(Message)
        {
        case STL::PROM::INIT:
        {
            STL::PINFO* Info = (STL::PINFO*)Input;
            Info->Type = STL::PROT::WINDOWED;
            Info->Depth = 1;
            Info->Left = 360;
            Info->Top = 200;
            Info->Width = (GRID_WIDTH + 2) * BLOCK_SIZE;
            Info->Height = (GRID_HEIGHT + 2) * BLOCK_SIZE;
            Info->Title = "Tetris";

        }
        break;
        case STL::PROM::DRAW:
        {
            STL::Framebuffer* Buffer = (STL::Framebuffer*)Input;

            DrawGridOutline(Buffer);
        }
        break;
        /*case STL::PROM::KEYPRESS:
        {
            uint8_t Key = *(uint8_t*)Input;
            
            return STL::PROR::DRAW;
        }
        break;        
        case STL::PROM::MOUSE:
        {
            STL::MINFO MouseInfo = *(STL::MINFO*)Input;

            return STL::PROR::DRAW;
        }
        break;*/
        default:
        {

        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}