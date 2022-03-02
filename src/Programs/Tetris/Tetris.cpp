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
#define GRID_OUTLINE_SHADOW STL::ARGB(60)
#define GRID_OUTLINE_HIGHLIGHT STL::ARGB(255)

#define LIGHT_BLUE_COLOR STL::ARGB(255, 0, 255, 255)
#define LIGHT_BLUE_SHADOW STL::ARGB(255, 0, 180, 179)
#define LIGHT_BLUE_HIGHLIGHT STL::ARGB(255, 180, 255, 255)

#define DARK_BLUE_COLOR STL::ARGB(255, 0, 0, 171)
#define DARK_BLUE_SHADOW STL::ARGB(255, 0, 0, 120)
#define DARK_BLUE_HIGHLIGHT STL::ARGB(255, 180, 181, 231)

#define ORANGE_COLOR STL::ARGB(255, 255, 120, 0)
#define ORANGE_SHADOW STL::ARGB(255, 179, 82, 0)
#define ORANGE_HIGHLIGHT STL::ARGB(255, 255, 215, 180)

#define YELLOW_COLOR STL::ARGB(255, 255, 255, 0)
#define YELLOW_SHADOW STL::ARGB(255, 179, 180, 0)
#define YELLOW_HIGHLIGHT STL::ARGB(255, 255, 255, 180)

#define PURPLE_COLOR STL::ARGB(255, 205, 0, 205)
#define PURPLE_SHADOW STL::ARGB(255, 143, 0, 143)
#define PURPLE_HIGHLIGHT STL::ARGB(255, 240, 181, 240)

#define GREEN_COLOR STL::ARGB(255, 0, 255, 0)
#define GREEN_SHADOW STL::ARGB(255, 0, 180, 0)
#define GREEN_HIGHLIGHT STL::ARGB(255, 179, 255, 179)

#define RED_COLOR STL::ARGB(255, 255, 0, 0)
#define RED_SHADOW STL::ARGB(255, 181, 0, 0)
#define RED_HIGHLIGHT STL::ARGB(255, 255, 181, 180)

#define START_FALL_RATE 100

namespace Tetris
{
    enum class BlockType
    {
        EMPTY,
        GRID_OUTLINE,   
        LIGHT_BLUE,
        DARK_BLUE, 
        ORANGE,
        YELLOW,
        PURPLE,
        GREEN,
        RED
    };

    BlockType Grid[GRID_WIDTH * GRID_HEIGHT];

    uint64_t CurrentFallRate = START_FALL_RATE;

    uint64_t NewPieceSeed = 1234;

    STL::Point GetScreenPosition(STL::Point Pos)
    {
        return (Pos + 1) * BLOCK_SIZE;
    }

    void DrawBlock(STL::Framebuffer* Buffer, STL::Point GridPos, STL::ARGB Color, STL::ARGB Shadow, STL::ARGB HighLight)
    {
        STL::Point ScreenPos = GetScreenPosition(GridPos);

        Buffer->DrawRectEdge(ScreenPos + RAISED_WIDTH, ScreenPos + BLOCK_SIZE - RAISED_WIDTH, HighLight, Shadow);
        Buffer->DrawRect(ScreenPos + RAISED_WIDTH, ScreenPos + BLOCK_SIZE - RAISED_WIDTH, Color);
    }

    void DrawBlock(STL::Framebuffer* Buffer, STL::Point GridPos, BlockType Type)
    {
        switch (Type)
        {
        case BlockType::GRID_OUTLINE:
        {
            DrawBlock(Buffer, GridPos, GRID_OUTLINE_COLOR, GRID_OUTLINE_SHADOW, GRID_OUTLINE_HIGHLIGHT);
        }   
        break;
        case BlockType::LIGHT_BLUE:
        {
            DrawBlock(Buffer, GridPos, LIGHT_BLUE_COLOR, LIGHT_BLUE_SHADOW, LIGHT_BLUE_HIGHLIGHT);
        }   
        break;        
        case BlockType::DARK_BLUE:
        {
            DrawBlock(Buffer, GridPos, DARK_BLUE_COLOR, DARK_BLUE_SHADOW, DARK_BLUE_HIGHLIGHT);
        }   
        break;           
        case BlockType::ORANGE:
        {
            DrawBlock(Buffer, GridPos, ORANGE_COLOR, ORANGE_SHADOW, ORANGE_HIGHLIGHT);
        }   
        break;             
        case BlockType::YELLOW:
        {
            DrawBlock(Buffer, GridPos, YELLOW_COLOR, YELLOW_SHADOW, YELLOW_HIGHLIGHT);
        }   
        break;             
        case BlockType::PURPLE:
        {
            DrawBlock(Buffer, GridPos, PURPLE_COLOR, PURPLE_SHADOW, PURPLE_HIGHLIGHT);
        }   
        break;         
        case BlockType::GREEN:
        {
            DrawBlock(Buffer, GridPos, GREEN_COLOR, GREEN_SHADOW, GREEN_HIGHLIGHT);
        }   
        break;         
        case BlockType::RED:
        {
            DrawBlock(Buffer, GridPos, RED_COLOR, RED_SHADOW, RED_HIGHLIGHT);
        }   
        break;
        case BlockType::EMPTY:
        {        
            STL::Point ScreenPos = GetScreenPosition(GridPos);
            Buffer->DrawRect(ScreenPos, ScreenPos + BLOCK_SIZE, STL::ARGB(0));
        }   
        break;
        }
    }

    void WritePiece(STL::Point GridPos, BlockType Type, BlockType ColorType)
    {
        switch (Type)
        {
        case BlockType::LIGHT_BLUE:
        {    
            const STL::Point Piece[] = {STL::Point(-1, 0), STL::Point(0, 0), STL::Point(1, 0), STL::Point(2, 0)};

            for (uint32_t i = 0; i < 4; i++)
            {
                STL::Point FinalPos = GridPos + Piece[i];
                Grid[FinalPos.X + FinalPos.Y * GRID_WIDTH] = ColorType;
            }
        }   
        break;        
        case BlockType::DARK_BLUE:
        {
            const STL::Point Piece[] = {STL::Point(0, 1), STL::Point(0, 0), STL::Point(1, 0), STL::Point(2, 0)};

            for (uint32_t i = 0; i < 4; i++)
            {
                STL::Point FinalPos = GridPos + Piece[i];
                Grid[FinalPos.X + FinalPos.Y * GRID_WIDTH] = ColorType;
            }
        }   
        break;           
        case BlockType::ORANGE:
        {
            const STL::Point Piece[] = {STL::Point(-1, 0), STL::Point(0, 0), STL::Point(1, 0), STL::Point(1, 1)};

            for (uint32_t i = 0; i < 4; i++)
            {
                STL::Point FinalPos = GridPos + Piece[i];
                Grid[FinalPos.X + FinalPos.Y * GRID_WIDTH] = ColorType;
            }
        }   
        break;             
        case BlockType::YELLOW:
        {
            const STL::Point Piece[] = {STL::Point(1, 1), STL::Point(0, 0), STL::Point(1, 0), STL::Point(0, 1)};

            for (uint32_t i = 0; i < 4; i++)
            {
                STL::Point FinalPos = GridPos + Piece[i];
                Grid[FinalPos.X + FinalPos.Y * GRID_WIDTH] = ColorType;
            }
        }   
        break;             
        case BlockType::PURPLE:
        {
            const STL::Point Piece[] = {STL::Point(-1, 0), STL::Point(0, 0), STL::Point(0, 1), STL::Point(1, 0)};

            for (uint32_t i = 0; i < 4; i++)
            {
                STL::Point FinalPos = GridPos + Piece[i];
                Grid[FinalPos.X + FinalPos.Y * GRID_WIDTH] = ColorType;
            }
        }   
        break;         
        case BlockType::GREEN:
        {
            const STL::Point Piece[] = {STL::Point(-1, 0), STL::Point(0, 0), STL::Point(0, 1), STL::Point(1, 1)};

            for (uint32_t i = 0; i < 4; i++)
            {
                STL::Point FinalPos = GridPos + Piece[i];
                Grid[FinalPos.X + FinalPos.Y * GRID_WIDTH] = ColorType;
            }
        }   
        break;         
        case BlockType::RED:
        {
            const STL::Point Piece[] = {STL::Point(-1, 1), STL::Point(0, 1), STL::Point(0, 0), STL::Point(1, 0)};

            for (uint32_t i = 0; i < 4; i++)
            {
                STL::Point FinalPos = GridPos + Piece[i];
                Grid[FinalPos.X + FinalPos.Y * GRID_WIDTH] = ColorType;
            }
        }   
        break;
        default:
        {        

        }   
        break;
        }
    }

    void DrawGridOutline(STL::Framebuffer* Buffer)
    {
        for (int i = -1; i < GRID_HEIGHT + 1; i ++)
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

    void DrawGrid(STL::Framebuffer* Buffer)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            for (int y = 0; y < GRID_HEIGHT; y++)
            {
                DrawBlock(Buffer, STL::Point(x, y), Grid[x + y * GRID_WIDTH]);
            }
        }
    }

    void ClearGrid()
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            for (int y = 0; y < GRID_HEIGHT; y++)
            {
                Grid[x + y * GRID_WIDTH] = BlockType::EMPTY;
            }
        }
    }

    namespace CurrentPiece
    {
        STL::Point Position;
        BlockType Type;

        void Move(STL::Point Offset)
        {
            WritePiece(Position, Type, BlockType::EMPTY);
            Position += Offset;
            WritePiece(Position, Type, Type);
        }

        void Generate()
        {            
            WritePiece(Position, Type, BlockType::EMPTY);

            Position = STL::Point(GRID_WIDTH / 2, 5);
            Type = (BlockType)(NewPieceSeed % 8 + 1);

            WritePiece(Position, Type, Type);
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

            CurrentPiece::Type = BlockType::EMPTY;

            ClearGrid();
        }
        break;
        case STL::PROM::DRAW:
        {
            STL::Framebuffer* Buffer = (STL::Framebuffer*)Input;
        
            DrawGridOutline(Buffer);

            DrawGrid(Buffer);
        }
        break;        
        case STL::PROM::TICK:
        {   
            uint64_t CurrentTick = *(uint64_t*)Input;

            NewPieceSeed += CurrentTick * 5231;

            if (CurrentTick % CurrentFallRate == 0)
            {   
                CurrentPiece::Move(STL::Point(0, 1));
                return STL::PROR::DRAW;
            }
        }
        break;
        case STL::PROM::KEYPRESS:
        {
            uint8_t Key = *(uint8_t*)Input;

            CurrentPiece::Generate();

            return STL::PROR::DRAW;
        }
        break;        
        case STL::PROM::MOUSE:
        {
            STL::MINFO MouseInfo = *(STL::MINFO*)Input;

            return STL::PROR::DRAW;
        }
        break;
        default:
        {

        }
        break;
        }

        return STL::PROR::SUCCESS;
    }
}