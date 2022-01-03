#pragma once

#include <stdint.h>

struct ARGB
{
	uint8_t B;
	uint8_t G;
	uint8_t R;
	uint8_t A;

	uint32_t ToInt();

	ARGB() = default;
	ARGB(uint8_t A, uint8_t R, uint8_t G, uint8_t B);
	ARGB(uint8_t T);
}__attribute__((packed));