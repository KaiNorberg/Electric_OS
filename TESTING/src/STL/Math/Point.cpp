#include "Point.h"

namespace STL
{ 		
    void Point::operator+=(Point const& Other)
    {
        this->X += Other.X;
        this->Y += Other.Y;
    }

    void Point::operator-=(Point const& Other)
    {
        this->X -= Other.X;
        this->Y -= Other.Y;
    }

    void Point::operator*=(Point const& Other)
    {
        this->X *= Other.X;
        this->Y *= Other.Y;
    }

    void Point::operator/=(Point const& Other)
    {
        this->X /= Other.X;
        this->Y /= Other.Y;
    }

    Point Point::operator+(Point const& Other)
    {
        return Point(this->X + Other.X, this->Y + Other.Y);
    }

    Point Point::operator-(Point const& Other)
    {
        return Point(this->X - Other.X, this->Y - Other.Y);
    }

    Point Point::operator*(Point const& Other)
    {
        return Point(this->X * Other.X, this->Y * Other.Y);
    }

    Point Point::operator/(Point const& Other)
    {
        return Point(this->X / Other.X, this->Y / Other.Y);
    }

    void Point::operator+=(int32_t const& Other)
    {
        this->X += Other;
        this->Y += Other;
    }

    void Point::operator-=(int32_t const& Other)
    {
        this->X -= Other;
        this->Y -= Other;
    }

    void Point::operator*=(int32_t const& Other)
    {
        this->X *= Other;
        this->Y *= Other;
    }

    void Point::operator/=(int32_t const& Other)
    {
        this->X /= Other;
        this->Y /= Other;
    }

    Point Point::operator+(int32_t const& Other)
    {
        return Point(this->X + Other, this->Y + Other);
    }

    Point Point::operator-(int32_t const& Other)
    {
        return Point(this->X - Other, this->Y - Other);
    }

    Point Point::operator*(int32_t const& Other)
    {
        return Point(this->X * Other, this->Y * Other);
    }

    Point Point::operator/(int32_t const& Other)
    {
        return Point(this->X / Other, this->Y / Other);
    }

    Point::Point(int32_t X, int32_t Y)
    {
        this->X = X;
        this->Y = Y;
    }
}