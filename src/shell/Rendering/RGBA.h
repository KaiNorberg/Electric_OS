#pragma once

#include <stdint.h>

struct RGBA
{
	uint8_t A;
	uint8_t R;
	uint8_t G;
	uint8_t B;

	uint32_t ToInt();

	RGBA(uint8_t R, uint8_t G, uint8_t B, uint8_t A);
	RGBA(uint8_t T);
}__attribute__((packed));