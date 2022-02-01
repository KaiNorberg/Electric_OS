#pragma once

#include <stdint.h>

#include "ARGB.h"
#include "STL/Math/Point.h"

#define RAISEDWIDTH 4
#define RAISEDHIGHCOLOR STL::ARGB(255)
#define RAISEDLOWCOLOR STL::ARGB(60)

namespace STL
{
	struct Framebuffer
	{
		ARGB* Base;
		uint64_t Size;
		uint32_t Width;
		uint32_t Height;
		uint32_t PixelsPerScanline;

		void Clear();

    	void ScrollUp(uint64_t Amount);

		ARGB GetPixel(Point Pixel);

		void PutPixel(Point Pixel, ARGB Color);

		void DrawRaisedRectEdge(Point TopLeft, Point BottomRight);

		void DrawRaisedRect(Point TopLeft, Point BottomRight, ARGB Color);

		void DrawSunkenRectEdge(Point TopLeft, Point BottomRight);

		void DrawSunkenRect(Point TopLeft, Point BottomRight, ARGB Color);

		void DrawRect(Point TopLeft, Point BottomRight, ARGB Color);

    	void Fill(ARGB Color);

		void PutChar(char chr, Point Pos, uint8_t Scale, ARGB Foreground, ARGB Background);

    	void Print(const char* cstr, Point& Pos, uint8_t Scale = 1, ARGB Foreground = ARGB(255), ARGB Background = ARGB(0), uint64_t NewLineOffset = 0);
	};
}