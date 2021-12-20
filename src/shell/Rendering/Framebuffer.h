#pragma once

#include <stdint.h>

#include "RGBA.h"

struct Framebuffer
{
	RGBA* Base;
	uint64_t Size;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanline;
} __attribute__((packed));
