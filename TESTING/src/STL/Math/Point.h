#pragma once

#include <stdint.h>

namespace STL
{ 
    struct Point
    {
        int32_t X;
        int32_t Y;

		void operator+=(Point const& Other);

		void operator-=(Point const& Other);

		void operator*=(Point const& Other);

		void operator/=(Point const& Other);

		Point operator+(Point const& Other);

		Point operator-(Point const& Other);

		Point operator*(Point const& Other);

		Point operator/(Point const& Other);

		void operator+=(int32_t const& Other);

		void operator-=(int32_t const& Other);

		void operator*=(int32_t const& Other);

		void operator/=(int32_t const& Other);

		Point operator+(int32_t const& Other);

		Point operator-(int32_t const& Other);

		Point operator*(int32_t const& Other);

		Point operator/(int32_t const& Other);

        Point() = default;
        Point(int32_t X, int32_t Y);
    } ;  
}