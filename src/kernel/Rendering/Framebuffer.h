#pragma once

#include <stdint.h>

#include "ARGB.h"

struct Framebuffer
{
	ARGB* Base;
	uint64_t Size;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanline;

	void SetPixel(uint64_t X, uint64_t Y, ARGB Color);
} __attribute__((packed));
