#pragma once

#include <stdint.h>

#include "ARGB.h"
#include "STL/Math/Point.h"

namespace STL
{
	struct Framebuffer
	{
		ARGB* Base;
		uint64_t Size;
		uint32_t Width;
		uint32_t Height;
		uint32_t PixelsPerScanline;
	};
	}