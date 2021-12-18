#pragma once

#include <stdint.h>

struct Framebuffer
{
	uint32_t* Base;
	uint64_t Size;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanline;
};
