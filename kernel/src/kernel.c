
typedef unsigned long long size_t;

typedef struct
{
	void* Base;
	size_t Size;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanline;
} Framebuffer;

typedef struct
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
} PSF1_HEADER;

typedef struct
{
	PSF1_HEADER* psf1_header;
	void* glyphBuffer;
} PSF1_FONT;

typedef struct
{
	unsigned int X;
    unsigned int Y;
} Point;

Point CursorPos;

void PutChar(Framebuffer* framebuffer, PSF1_FONT* PSF1_Font, unsigned int Color, char chr, Point Pos)
{
    unsigned int* PixelPtr = (unsigned int*)framebuffer->Base;
    char* FontPtr = PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_header->charsize);

    if (Pos.X > framebuffer->Width)
    {
        Pos.X = 0;
        Pos.Y += 16;
    }

    for (size_t y = Pos.Y; y < Pos.Y + 16; y++)
    {
        for (size_t x = Pos.X; x < Pos.X + 8; x++)
        {
            if ((*FontPtr & (0b10000000 >> (x - Pos.X))) > 0)
            {
                *(unsigned int*)(PixelPtr + x + (y * framebuffer->PixelsPerScanline)) = Color;
            }
        }
        FontPtr++;
    }
}

void Print(Framebuffer* framebuffer, PSF1_FONT* PSF1_Font, unsigned int Color, char* str)
{
    char* chr = str;

    while (*chr != 0)
    {
        if (*chr == '\n')
        {
            CursorPos.Y += 16;
        }
        else if (*chr == '\r')
        {
            CursorPos.X = 0;
        }
        else
        {
            PutChar(framebuffer, PSF1_Font, Color, *chr, CursorPos);
            CursorPos.X += 8;
        }
        chr++;
    }
}

void _start(Framebuffer* framebuffer, PSF1_FONT* PSF1_Font)
{
    Print(framebuffer, PSF1_Font, 0xFFFFFFFF, "Hello, World!");
    return;
}
