#include "ARGB.h"

namespace STL
{		
    uint32_t ARGB::ToInt()
    {
        uint32_t A32 = A;
        uint32_t R32 = R;
        uint32_t G32 = G;
        uint32_t B32 = B;

        return B32 + (G32 << 8) + (R32 << 16) + (A32 << 24);
    }

	bool ARGB::operator==(ARGB const& Other)
    {
        return (this->A == Other.A || this->R == Other.R || this->G == Other.G || this->B == Other.B);
    }

    void ARGB::operator+=(ARGB const& Other)
    {
        this->A += Other.A;
        this->R += Other.R;
        this->G += Other.G;
        this->B += Other.B;
    }

    void ARGB::operator-=(ARGB const& Other)
    {
        this->A -= Other.A;
        this->R -= Other.R;
        this->G -= Other.G;
        this->B -= Other.B;
    }

    void ARGB::operator*=(ARGB const& Other)
    {
        this->A *= Other.A;
        this->R *= Other.R;
        this->G *= Other.G;
        this->B *= Other.B;
    }

    void ARGB::operator/=(ARGB const& Other)
    {
        this->A /= Other.A;
        this->R /= Other.R;
        this->G /= Other.G;
        this->B /= Other.B;
    }

    ARGB ARGB::operator+(ARGB const& Other)
    {
        return ARGB(this->A + Other.A, this->R + Other.R, this->G + Other.G, this->B + Other.B);
    }

    ARGB ARGB::operator-(ARGB const& Other)
    {
        return ARGB(this->A - Other.A, this->R - Other.R, this->G - Other.G, this->B - Other.B);
    }

    ARGB ARGB::operator*(ARGB const& Other)
    {
        return ARGB(this->A * Other.A, this->R * Other.R, this->G * Other.G, this->B * Other.B);
    }

    ARGB ARGB::operator/(ARGB const& Other)
    {
        return ARGB(this->A / Other.A, this->R / Other.R, this->G / Other.G, this->B / Other.B);
    }

    void ARGB::operator+=(uint8_t const& Other)
    {
        this->A += Other;
        this->R += Other;
        this->G += Other;
        this->B += Other;        
    }

    void ARGB::operator-=(uint8_t const& Other)
    {
        this->A -= Other;
        this->R -= Other;
        this->G -= Other;
        this->B -= Other;   
    }

    void ARGB::operator*=(uint8_t const& Other)
    {
        this->A *= Other;
        this->R *= Other;
        this->G *= Other;
        this->B *= Other;   
    }

    void ARGB::operator/=(uint8_t const& Other)
    {
        this->A /= Other;
        this->R /= Other;
        this->G /= Other;
        this->B /= Other;   
    }

    ARGB ARGB::operator+(uint8_t const& Other)
    {
        return ARGB(this->A + Other, this->R + Other, this->G + Other, this->B + Other);
    }

    ARGB ARGB::operator-(uint8_t const& Other)
    {
        return ARGB(this->A - Other, this->R - Other, this->G - Other, this->B - Other);
    }

    ARGB ARGB::operator*(uint8_t const& Other)
    {
        return ARGB(this->A * Other, this->R * Other, this->G * Other, this->B * Other);
    }

    ARGB ARGB::operator/(uint8_t const& Other)
    {
        return ARGB(this->A / Other, this->R / Other, this->G / Other, this->B / Other);
    }

    ARGB::ARGB(uint8_t A, uint8_t R, uint8_t G, uint8_t B)
    {
        this->A = A;
        this->R = R;
        this->G = G;
        this->B = B;
    }

    ARGB::ARGB(uint8_t X)
    {    
        this->A = X;
        this->R = X;
        this->G = X;
        this->B = X;
    }
}