#pragma once

#include <stdint.h>

namespace STL
{
	struct ARGB
	{
		uint8_t B;
		uint8_t G;
		uint8_t R;
		uint8_t A;

		uint32_t ToInt();

		void operator+=(ARGB const& Other);

		void operator-=(ARGB const& Other);

		void operator*=(ARGB const& Other);

		void operator/=(ARGB const& Other);

		ARGB operator+(ARGB const& Other);

		ARGB operator-(ARGB const& Other);

		ARGB operator*(ARGB const& Other);

		ARGB operator/(ARGB const& Other);

		void operator+=(uint8_t const& Other);

		void operator-=(uint8_t const& Other);

		void operator*=(uint8_t const& Other);

		void operator/=(uint8_t const& Other);

		ARGB operator+(uint8_t const& Other);

		ARGB operator-(uint8_t const& Other);

		ARGB operator*(uint8_t const& Other);

		ARGB operator/(uint8_t const& Other);

		ARGB() = default;

		ARGB(uint8_t A, uint8_t R, uint8_t G, uint8_t B);

		ARGB(uint8_t X);
		
	}__attribute__((packed));
}