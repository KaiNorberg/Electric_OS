#pragma once

#include <stdint.h>

struct Framebuffer
{
	unsigned int* Base;
	uint64_t Size;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanline;
};
